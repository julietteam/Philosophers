/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/12 18:56:57 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	check_philosopher_status(t_philosopher *philosopher)
{
	long long	time_since_last_meal;
	int			timed_out;
	int			dead;
	int			stop;
	
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	time_since_last_meal = current_timestamp_in_ms() - philosopher->last_meal_time;
	timed_out = time_since_last_meal > philosopher->params.time_to_die;
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (timed_out && !dead && !stop)
	{

		if (display_log(philosopher->simulation, philosopher->id, "died", philosopher) == -1)	
		{
			pthread_mutex_lock(&philosopher->simulation->death);
			philosopher->is_dead = 1;
			pthread_mutex_unlock(&philosopher->simulation->death);
			return(-1);
		}
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		philosopher->simulation->stop = 1;
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return (-1);
	}
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->simulation->stop)
	{
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0); 
}


int	monitor_philosopher_cycle(t_philosopher *philosopher)
{
	
	pthread_mutex_lock(&philosopher->simulation->death);
	int dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (!dead && !stop)
	{
		usleep(5000);
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		int full = philosopher->full;
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

	philosopher = (t_philosopher *)arg;

	pthread_mutex_lock(&philosopher->simulation->death);
	int dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
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
	if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].thread_launched = false;
		return (false);
	}
	simulation->philosophers[i].thread_launched = true;

	
	if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, monitor_philosopher, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].monitor_launched = false;
		return (false);
	}
	simulation->philosophers[i].monitor_launched = true;
	
	return (true);
}

void	handle_thread_creation_error(t_simulation *simulation, int philosopher_count)
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
		handle_thread_creation_error(simulation, philosopher_count);
	}
	return (0);
}

