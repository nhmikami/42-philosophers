/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:22:34 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/11 19:22:12 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_action
{
	TAKE_FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
}	t_action;

typedef struct s_data
{
	int			num_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_must_eat;
	int			start_time;
	int			stop_sim;
	pid_t		*philo_pid;
	sem_t		*fork_sem;
	sem_t		*print_sem;
	sem_t		*full_sem;
	sem_t		*starve_sem;
	sem_t		*stop_sem;
	pthread_t	starvation;
	pthread_t	all_full;
}	t_data;

typedef struct s_philo
{
	int			id;
	int			meals_taken;
	int			last_meal;
	char		*sem_name;
	sem_t		*meal_sem;
	pthread_t	monitor;
	t_data		*data;
}	t_philo;

// initialization
int		get_args(int ac, char **av, t_data *data);
int		init_data(t_data *data);
void	init_philo(t_philo *philo, t_data *data, int i);
void	close_data_semaphores(t_data *data, int stage);
void	unlink_data_semaphores(void);

// simulation
void	start_simulation(t_data *data);
int		stop_simulation(t_data *data, int stop);
void	print_action(t_philo *philo, t_action action);
void	philo_routine(t_data *data, int i);
void	*all_full(void *arg);
void	*starvation(void *arg);

// utils
int		get_current_time(void);
int		time_elapsed(int start_time);
void	ft_usleep(int milliseconds);
void	kill_all_philos(t_data *data);
char	*ft_itoa(int n);
char	*ft_strjoin(char const *s1, char const *s2);

#endif