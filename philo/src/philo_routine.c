/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:29 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/10 14:50:30 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, TAKE_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, TAKE_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, TAKE_FORK);
	}
}

static void	*eating(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_current_time();
	print_action(philo, EAT);
	philo->meals_taken++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

static void	*sleeping(t_philo *philo)
{
	print_action(philo, SLEEP);
	ft_usleep(philo->data->time_to_sleep);
	return (NULL);
}

static void	*thinking(t_philo *philo)
{
	print_action(philo, THINK);
	usleep(500);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		print_action(philo, TAKE_FORK);
		ft_usleep(philo->data->time_to_die);
		return (NULL);
	}
	while (!stop_simulation(philo, 0))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}
