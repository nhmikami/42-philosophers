#include "../inc/philo.h"

void	print_action(t_philo *philo, t_action action)
{
	int	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	timestamp = time_elapsed(philo->data->start_time);
	if (!stop_simulation(philo, 0))
	{
		if (action == TAKE_FORK)
			printf("%d %d has taken a fork\n", timestamp, philo->id);
		else if (action == EAT)
			printf("%d %d is eating\n", timestamp, philo->id);
		else if (action == SLEEP)
			printf("%d %d is sleeping\n", timestamp, philo->id);
		else if (action == THINK)
			printf("%d %d is thinking\n", timestamp, philo->id);
		else if (action == DIE)
			printf("%d %d died\n", timestamp, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

static void	start_threads(t_philo *philo, pthread_t *monitoring)
{
	int	i;

	i = 0;
	while (i < philo->data->num_philos)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_routine,
				(void *)&philo[i]))
		{
			printf("Error: failed to create philosophers thread\n");
			return ;
		}
		i++;
	}
	if (pthread_create(monitoring, NULL, monitor_routine, (void *)philo))
		printf("Error: failed to create monitor thread\n");
}

static void	join_threads(t_philo *philo, pthread_t monitoring)
{
	int	i;

	i = 0;
	while (i < philo->data->num_philos)
	{
		if (pthread_join(philo[i].thread, NULL))
			printf("Error: failed to join thread\n");
		i++;
	}
	if (pthread_join(monitoring, NULL))
		printf("Error: failed to join thread\n");
}

void	start_simulation(t_philo *philo)
{
	pthread_t	monitoring;

	start_threads(philo, &monitoring);
	join_threads(philo, monitoring);
}

int	stop_simulation(t_philo *philo, int stop)
{
	int	flag;

	pthread_mutex_lock(&philo->data->stop_lock);
	if (stop)
		philo->data->stop_sim = stop;
	flag = philo->data->stop_sim;
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (flag);
}
