/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/15 17:26:12 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_second_forks(t_philosopher *philosopher, \
pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(second_fork);
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, \
	philosopher->id, "has taken a fork", philosopher) == -1)
	{	
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (stop(philosopher))
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	return (0);
}

int	take_forks(t_philosopher *philosopher, \
pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, \
	philosopher->id, "has taken a fork", philosopher) == -1)
	{	
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (take_second_forks(philosopher, first_fork, second_fork) == -1)
		return (-1);
	return (0);
}

void	remaining_to_eat(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = start_time;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
	}
	philosopher->meals_eaten++;
}

int	eat(t_philosopher *philosopher, \
pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "is eating", \
	philosopher) == -1)
		return (-1);
	remaining_to_eat(philosopher);
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	return (0);
}

int	think_and_sleep(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	if (stop(philosopher))
		return (-1);
	if (display_log(philosopher->simulation, philosopher->id, "is sleeping", \
	philosopher) == -1)
		return (-1);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	if (stop(philosopher))
		return (-1);
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", \
	philosopher) == -1)
		return (-1);
	return (0);
}

