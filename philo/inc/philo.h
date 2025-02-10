#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
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
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
	int				start_time;
	int				stop_sim;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_taken;
	int				last_meal;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	meal_lock;
	t_data			*data;
}	t_philo;

// initialization
int		get_args(int ac, char **av, t_data *data);
int		init_data(t_data *data);
int		init_philos(t_philo *philo, t_data *data);
void	destroy_data_mtx(t_data *data, int stage);

// simulation
void	start_simulation(t_philo *philo);
int		stop_simulation(t_philo *philo, int stop);
void	print_action(t_philo *philo, t_action action);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

// utils
int		get_current_time(void);
int		time_elapsed(int start_time);
void	ft_usleep(int milliseconds);
void	free_all(t_philo *philo);

#endif