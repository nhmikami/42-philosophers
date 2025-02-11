/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:20:58 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 19:46:48 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

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

void	kill_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (waitpid(data->philo_pid[i], NULL, WNOHANG) == 0)
			kill(data->philo_pid[i], SIGKILL);
		i++;
	}
}
