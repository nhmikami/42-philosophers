/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:22:43 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/12 17:46:18 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

int	main(int ac, char **av)
{
	t_data	data;
	int		status;

	if (!get_args(ac, av, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	start_simulation(&data);
	pthread_create(&data.starvation, NULL, &starvation, (void *)&data);
	pthread_create(&data.all_full, NULL, &all_full, (void *)&data);
	pthread_join(data.starvation, NULL);
	pthread_join(data.all_full, NULL);
	while (waitpid(-1, &status, 0) != -1)
		;
	close_data_semaphores(&data, 6);
	unlink_data_semaphores();
	free(data.philo_pid);
	return (0);
}
