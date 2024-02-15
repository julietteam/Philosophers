/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:09:48 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/15 18:51:39 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitor_philosopher_cycle(t_philosopher *philosopher)
{
	int	dead;
	int	stop;
	int	full;

	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (!dead && !stop)
	{
		usleep(5000);
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		full = philosopher->full;
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		if (full)
			return (-1);
		if (check_philosopher_status(philosopher) == -1)
			return (-1);
	}
	return (0);
}

void	*monitor_philosopher(void *arg)
{
	t_philosopher	*philosopher;
	int				dead;
	int				stop;

	philosopher = (t_philosopher *)arg;
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (!dead && !stop)
	{
		if (monitor_philosopher_cycle(philosopher) == -1)
			break ;
	}
	return (NULL);
}

bool	create_philosopher_thread(t_simulation *simulation, int i)
{
	if (pthread_create(&simulation->philosophers[i].thread, NULL, \
	philosopher_routine, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].thread_launched = false;
		return (false);
	}
	simulation->philosophers[i].thread_launched = true;
	if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, \
	monitor_philosopher, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].monitor_launched = false;
		return (false);
	}
	simulation->philosophers[i].monitor_launched = true;
	return (true);
}

void	creation_error(t_simulation *simulation, int philosopher_count)
{
	int	j;

	j = 0;
	while (j < philosopher_count)
	{
		if (simulation->philosophers[j].thread_launched)
		{
			pthread_join(simulation->philosophers[j].thread, NULL);
		}
		if (simulation->philosophers[j].monitor_launched)
		{
			pthread_join(simulation->philosophers[j].monitor_thread, NULL);
		}
		j++;
	}
	free_simulation(simulation);
}

int	start_philosopher_threads(t_simulation *simulation)
{
	int		philosopher_count;
	int		i;
	bool	error_occurred;

	philosopher_count = simulation->params->number_of_philosophers;
	error_occurred = false;
	i = 0;
	while (i < philosopher_count)
	{
		if (!create_philosopher_thread(simulation, i))
		{
			error_occurred = true;
			break ;
		}
		i++;
	}
	if (error_occurred)
	{
		creation_error(simulation, philosopher_count);
	}
	return (0);
}
