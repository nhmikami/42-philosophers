/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:22:43 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 16:47:24 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (!get_args(ac, av, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	pthread_create(&data.starvation, NULL, &starvation, (void *)&data);
	pthread_create(&data.all_full, NULL, &all_full, (void *)&data);
	start_simulation(&data);
	pthread_join(data.starvation, NULL);
	pthread_join(data.all_full, NULL);
	return (0);
}
