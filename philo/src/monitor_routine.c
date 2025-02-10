#include "../inc/philo.h"

static int	starvation(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->num_philos && !stop_simulation(philo, 0))
	{
		pthread_mutex_lock(&philo[i].meal_lock);
		if (time_elapsed(philo[i].last_meal) > philo->data->time_to_die)
		{
			print_action(&philo[i], DIE);
			pthread_mutex_unlock(&philo[i].meal_lock);
			stop_simulation(philo, 1);
			return (1);
		}
		pthread_mutex_unlock(&philo[i].meal_lock);
		i++;
	}
	return (0);
}

static int	all_full(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->num_philos && !stop_simulation(philo, 0))
	{
		pthread_mutex_lock(&philo[i].meal_lock);
		if (philo[i].meals_taken < philo->data->times_must_eat)
		{
			pthread_mutex_unlock(&philo[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&philo[i].meal_lock);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!stop_simulation(philo, 0))
	{
		if (starvation(philo))
			stop_simulation(philo, 1);
		if (philo->data->times_must_eat > 0 && all_full(philo))
			stop_simulation(philo, 1);
		usleep(500);
	}
	return (NULL);
}
