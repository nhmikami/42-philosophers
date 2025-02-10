#include "../inc/philo.h"

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philo;

	if (!get_args(ac, av, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	philo = malloc(sizeof(t_philo) * data.num_philos);
	if (!philo)
	{
		printf("Error: failed to allocate memory for philosophers\n");
		destroy_data_mtx(&data, 3);
		return (1);
	}
	if (!init_philos(philo, &data))
	{
		free(philo);
		return (1);
	}
	start_simulation(philo);
	free_all(philo);
	return (0);
}
