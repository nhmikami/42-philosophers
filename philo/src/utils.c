#include "../inc/philo.h"

int	get_current_time(void)
{
	struct timeval	tv;
	int				time;

	gettimeofday(&tv, NULL);
	time = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	return (time);
}

int	time_elapsed(int start_time)
{
	int	elapsed_time;

	elapsed_time = get_current_time() - start_time;
	return (elapsed_time);
}

void	ft_usleep(int milliseconds)
{
	int	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(10);
	return ;
}

void	free_all(t_philo *philo)
{
	int		i;

	i = 0;
	while (i < philo->data->num_philos)
	{
		pthread_mutex_destroy(&philo[i].meal_lock);
		i++;
	}
	destroy_data_mtx(philo->data, 3);
	free(philo);
}
