/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:50:13 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 16:33:54 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

static void	eating(t_philo *philo)
{
	sem_wait(philo->data->fork_sem);
	print_action(philo, TAKE_FORK);
	sem_wait(philo->data->fork_sem);
	print_action(philo, TAKE_FORK);
	sem_wait(philo->meal_sem);
	philo->last_meal = get_current_time();
	print_action(philo, EAT);
	sem_post(philo->meal_sem);
	philo->meals_taken++;
	if (philo->meals_taken == philo->data->times_must_eat)
		sem_post(philo->data->full_sem);
	ft_usleep(philo->data->time_to_eat);
	sem_post(philo->data->fork_sem);
	sem_post(philo->data->fork_sem);
}

static void	sleeping(t_philo *philo)
{
	print_action(philo, SLEEP);
	ft_usleep(philo->data->time_to_sleep);
}

static void	thinking(t_philo *philo)
{
	print_action(philo, THINK);
	usleep(500);
}

static void	*self_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!stop_simulation(philo->data, 0))
	{
		sem_wait(philo->meal_sem);
		if (time_elapsed(philo->last_meal) > philo->data->time_to_die)
		{
			print_action(philo, DIE);
			sem_post(philo->data->starve_sem);
			sem_post(philo->meal_sem);
			break ;
		}
		sem_post(philo->meal_sem);
		usleep(500);
	}
	return (NULL);
}

void	philo_routine(t_data *data, int i)
{
	t_philo		philo;

	init_philo(&philo, data, i);
	pthread_create(&philo.monitor, NULL, self_monitor, (void *)&philo);
	pthread_detach(philo.monitor);
	if (data->num_philos == 1)
	{
		print_action(&philo, TAKE_FORK);
		ft_usleep(philo.data->time_to_die);
	}
	if (philo.id % 2 == 0)
		thinking(&philo);
	while (!stop_simulation(data, 0))
	{
		eating(&philo);
		sleeping(&philo);
		thinking(&philo);
	}
	sem_close(philo.meal_sem);
	sem_unlink(philo.sem_name);
	free(philo.sem_name);
}
