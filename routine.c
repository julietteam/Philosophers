/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/08 17:12:07 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if(philosopher->is_dead == 1)
		return (-1);
	pthread_mutex_lock(first_fork);
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{
		return (-1);
	}
	if(philosopher->is_dead == 1)
		return (-1);
	pthread_mutex_lock(second_fork);
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{
		return (-1);
	}
	pthread_mutex_unlock(second_fork);
	return (0);
	
}


int	eat(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	if(philosopher->is_dead == 1)
		return (-1);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = start_time;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (display_log(philosopher->simulation, philosopher->id, "is eating", philosopher) == -1)
	{
		return (-1);
	}	
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
		if(philosopher->is_dead == 1)
			return (-1);
	}
	philosopher->meals_eaten++;
	return(0);
}


int	think_and_sleep(t_philosopher *philosopher) 
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	if (display_log(philosopher->simulation, philosopher->id, "is sleeping", philosopher) == -1)
	{
		return (-1);
	}
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	while (remaining > 0)
	{
		usleep(100);
		if(philosopher->is_dead == 1)
			return (-1);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", philosopher) == -1)
	{
		return (-1);
	}	
	usleep(1000);
	return (0);
}


int	update_scheduler(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->params.number_of_times_each_philosopher_must_eat > 0 &&
	philosopher->meals_eaten >= philosopher->params.number_of_times_each_philosopher_must_eat)
	{
		philosopher->full = 1;
		philosopher->simulation->full_philosophers++;
		if (philosopher->simulation->full_philosophers == philosopher->params.number_of_philosophers)
		{
			return (-1);
		}
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0);
}






