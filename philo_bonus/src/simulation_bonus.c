/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:52:00 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 19:50:04 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

void	print_action(t_philo *philo, t_action action)
{
	int	timestamp;

	sem_wait(philo->data->print_sem);
	timestamp = time_elapsed(philo->data->start_time);
	if (!stop_simulation(philo->data, 0))
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
	sem_post(philo->data->print_sem);
}

void	*starvation(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	sem_wait(data->starve_sem);
	if (stop_simulation(data, 0))
		return (NULL);
	stop_simulation(data, 1);
	kill_all_philos(data);
	sem_post(data->full_sem);
	return (NULL);
}

void	*all_full(void *arg)
{
	t_data	*data;
	int		count_full;

	data = (t_data *)arg;
	count_full = 0;
	while (count_full < data->num_philos)
	{
		sem_wait(data->full_sem);
		if (stop_simulation(data, 0))
			return (NULL);
		count_full++;
	}
	stop_simulation(data, 1);
	kill_all_philos(data);
	sem_post(data->starve_sem);
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		data->philo_pid[i] = fork();
		if (data->philo_pid[i] < 0)
		{
			printf("Error: failed to create process\n");
			kill_all_philos(data);
			return ;
		}
		if (data->philo_pid[i] == 0)
			philo_routine(data, i);
		i++;
	}
	while (waitpid(-1, &status, 0) != -1)
		;
	close_data_semaphores(data, 6);
	unlink_data_semaphores();
	free(data->philo_pid);
}

int	stop_simulation(t_data *data, int stop)
{
	int	flag;

	sem_wait(data->stop_sem);
	if (stop)
		data->stop_sim = stop;
	flag = data->stop_sim;
	sem_post(data->stop_sem);
	return (flag);
}
