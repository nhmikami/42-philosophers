/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:22:43 by naharumi          #+#    #+#             */
/*   Updated: 2025/02/10 18:16:41 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo_bonus.h"

// simulation
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

// init_data
int init_data_semaphores(t_data *data)
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

// philo routine
void    init_philo(t_philo *philo, t_data *data, int i)
{
	char	*str_id;

	philo->id = i + 1;
	philo->meals_taken = 0;
	philo->last_meal = data->start_time;
	philo->data = data;
	str_id = ft_itoa(philo->id);
	philo->sem_name = ft_strjoin("state", str_id); // malloc
	free(str_id);
	sem_unlink(philo->sem_name);
	philo->meal_sem = sem_open(philo->sem_name, O_CREAT, 0660, 1);
}

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->data->fork_sem);
	print_action(philo, TAKE_FORK);
	sem_wait(philo->data->fork_sem);
	print_action(philo, TAKE_FORK);
}

static void	eating(t_philo *philo)
{
	take_forks(philo);
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

void	*self_monitor(void *arg)
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
			break;
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

// monitor_routine
void	kill_all_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		kill(data->philo_pid[i], SIGKILL);
		i++;
	}
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

// simulation
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

void	start_simulation(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_philos)
	{
		data->philo_pid[i] = fork();
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

// main
int main(int ac, char **av)
{
	t_data	data;

	if (!get_args(ac, av, &data))
		return (1);
	if (!init_data(&data)) // malloc data->philo_pid
		return (1);
	pthread_create(&data.starvation, NULL, &starvation, (void *)&data);
	pthread_create(&data.all_full, NULL, &all_full, (void *)&data);
	start_simulation(&data);
	pthread_join(data.starvation, NULL);
	pthread_join(data.all_full, NULL);
	return (0);
}


	/*int i;
	i = 0;
	while (i < data.num_philos)
	{
		data.philo_pid[i] = fork()
		if (data.philo_pid[i] == 0)
			philo_routine();
		i++;
	}*/