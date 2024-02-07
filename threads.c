/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/07 19:43:36 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


bool	check_philosopher_status(t_philosopher *philosopher)
{
	long long	time_since_last_meal;
	bool		timed_out;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->simulation->is_running)
	{
		time_since_last_meal = current_timestamp_in_ms() - philosopher->last_meal_time;
		timed_out = time_since_last_meal > philosopher->params.time_to_die;
		if (timed_out && !philosopher->is_dead)
		{
			pthread_mutex_lock(&philosopher->simulation->death);
			philosopher->is_dead = 1;
			pthread_mutex_unlock(&philosopher->simulation->death);
			pthread_mutex_lock(&philosopher->simulation->write);
			display_log(philosopher->simulation, philosopher->id, "died");
			pthread_mutex_unlock(&philosopher->simulation->write);
			philosopher->simulation->is_running = 0; 
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
			return (true);
		}
	
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (false);
}

bool	update_simulation_status(t_philosopher *philosopher)
{
	bool	is_running_local;

	//pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->is_dead)
	{
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		philosopher->simulation->is_running = 0;
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	}
	is_running_local = philosopher->simulation->is_running;
	return (is_running_local);
}

void	monitor_philosopher_cycle(t_philosopher *philosopher)
{
	bool	is_running_local;

	while (1)
	{
		usleep(5000);
		if (check_philosopher_status(philosopher))
		{
			is_running_local = update_simulation_status(philosopher);
			if (!is_running_local)
			{
				return ;
			}
		}
		else
		{
			pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
			is_running_local = philosopher->simulation->is_running;
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
			if (!is_running_local)
			{
				return ;
			}
		}
	}
}

void	*monitor_philosopher(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	monitor_philosopher_cycle(philosopher);
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
			pthread_join(simulation->philosophers[j].thread, NULL);
		if (simulation->philosophers[j].monitor_launched)
			pthread_join(simulation->philosophers[j].monitor_thread, NULL);
		j++;
	}
	exit_cleanly(simulation, philosopher_count);
}

void	start_all_philosopher_threads(t_simulation *simulation)
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
		handle_thread_creation_error(simulation, philosopher_count);
}

void	start_philosopher_threads(t_simulation *simulation)
{
	start_all_philosopher_threads(simulation);
}
