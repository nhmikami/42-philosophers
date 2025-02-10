#include "../inc/philo.h"

void	destroy_data_mtx(t_data *data, int stage)
{
	int	i;

	if (stage >= 2)
	{
		pthread_mutex_destroy(&data->print_lock);
		if (stage == 3)
			pthread_mutex_destroy(&data->stop_lock);
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

static int	init_data_mtx(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
		{
			printf("Error: failed to initialize mutex\n");
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			return (-1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->stop_lock, NULL))
		return (2);
	return (0);
}

int	init_data(t_data *data)
{
	int	stage;

	data->stop_sim = 0;
	data->start_time = get_current_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		printf("Error: failed to allocate memory for forks\n");
		return (0);
	}
	stage = init_data_mtx(data);
	if (stage != 0)
	{
		if (stage > 0)
		{
			printf("Error: failed to initialize mutex\n");
			destroy_data_mtx(data, stage);
		}
		return (0);
	}
	return (1);
}

int	init_philos(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_taken = 0;
		philo[i].last_meal = data->start_time;
		philo[i].data = data;
		philo[i].left_fork = &data->forks[i];
		philo[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		if (pthread_mutex_init(&philo[i].meal_lock, NULL))
		{
			printf("Error: failed to initialize mutex\n");
			while (--i >= 0)
				pthread_mutex_destroy(&philo[i].meal_lock);
			destroy_data_mtx(data, 3);
			return (0);
		}
		i++;
	}
	return (1);
}
