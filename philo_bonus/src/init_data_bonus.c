/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:51:15 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 16:35:53 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

void	close_data_semaphores(t_data *data, int stage)
{
	if (stage >= 2)
		sem_close(data->fork_sem);
	if (stage >= 3)
		sem_close(data->print_sem);
	if (stage >= 4)
		sem_close(data->full_sem);
	if (stage >= 5)
		sem_close(data->starve_sem);
	if (stage == 6)
		sem_close(data->stop_sem);
}

void	unlink_data_semaphores(void)
{
	sem_unlink("/fork");
	sem_unlink("/print");
	sem_unlink("/full");
	sem_unlink("/starve");
	sem_unlink("/stop");
}

static int	init_data_semaphores(t_data *data)
{
	data->fork_sem = sem_open("/fork", O_CREAT, 0660, data->num_philos);
	if (data->fork_sem == SEM_FAILED)
		return (1);
	data->print_sem = sem_open("/print", O_CREAT, 0660, 1);
	if (data->print_sem == SEM_FAILED)
		return (2);
	data->full_sem = sem_open("/full", O_CREAT, 0660, 0);
	if (data->full_sem == SEM_FAILED)
		return (3);
	data->starve_sem = sem_open("/starve", O_CREAT, 0660, 0);
	if (data->starve_sem == SEM_FAILED)
		return (4);
	data->stop_sem = sem_open("/stop", O_CREAT, 0660, 1);
	if (data->stop_sem == SEM_FAILED)
		return (5);
	return (0);
}

int	init_data(t_data *data)
{
	int	stage;

	data->stop_sim = 0;
	data->start_time = get_current_time();
	data->philo_pid = malloc(sizeof(pid_t) * data->num_philos);
	if (!data->philo_pid)
	{
		printf("Error: failed to allocate memory\n");
		return (0);
	}
	unlink_data_semaphores();
	stage = init_data_semaphores(data);
	if (stage > 0)
	{
		printf("Error: failed to start semaphore\n");
		close_data_semaphores(data, stage);
		unlink_data_semaphores();
		free(data->philo_pid);
		return (0);
	}
	return (1);
}

void	init_philo(t_philo *philo, t_data *data, int i)
{
	char	*str_id;

	philo->id = i + 1;
	philo->meals_taken = 0;
	philo->last_meal = data->start_time;
	philo->data = data;
	str_id = ft_itoa(philo->id);
	philo->sem_name = ft_strjoin("state", str_id);
	free(str_id);
	sem_unlink(philo->sem_name);
	philo->meal_sem = sem_open(philo->sem_name, O_CREAT, 0660, 1);
}
