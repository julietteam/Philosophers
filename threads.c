/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/08 18:28:38 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	check_philosopher_status(t_philosopher *philosopher)
{
	long long	time_since_last_meal;
	int			timed_out;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	time_since_last_meal = current_timestamp_in_ms() - philosopher->last_meal_time;
	timed_out = time_since_last_meal > philosopher->params.time_to_die;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	//printf("timed_out: %d\n", timed_out);
	//printf("Philo mort: %d\n", philosopher->is_dead);
	if (timed_out && !philosopher->is_dead)
	{
		pthread_mutex_lock(&philosopher->simulation->death);
		philosopher->is_dead = 1;
		pthread_mutex_unlock(&philosopher->simulation->death);
		if (display_log(philosopher->simulation, philosopher->id, "died", philosopher) == -1)
		{
			return (-1);
		}	
		// pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return (-1);
	}
	//pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0); 
}


void	monitor_philosopher_cycle(t_philosopher *philosopher)
{
	bool	is_running_local;
	while (1)
	{
		usleep(5000);
		printf("exit status: %d\n", philosopher->exit_status);
		if (check_philosopher_status(philosopher) == -1)
		{
			break ;
		}
		else
		{
			is_running_local = philosopher->exit_status;
			printf("exit status: %d\n", philosopher->exit_status);
			if (!is_running_local)
				break ;
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
		handle_thread_creation_error(simulation, philosopher_count);
	return (0);
}

