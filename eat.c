/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 18:06:36 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

int	remaining_to_eat(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = start_time;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (!stop(philosopher))
	{
		while (remaining > 0)
		{
			usleep(100);
			if (stop(philosopher))
				return (-1);
			pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
			elapsed = current_timestamp_in_ms() - start_time;
			remaining = philosopher->params.time_to_eat - elapsed;
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		}
	}
	philosopher->meals_eaten++;
	return (0);
}

int	eat(t_philosopher *philosopher, \
pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (stop(philosopher))
	{
		unlock_forks(first_fork, second_fork);
		return (-1);
	}
	if (!stop(philosopher))
	{
		if (display_log(philosopher->simulation, philosopher->id, "is eating", \
		philosopher) == -1)
			return (-1);
	}
	if (!stop(philosopher))
	{
		if (remaining_to_eat(philosopher) == -1)
		{
			unlock_forks(first_fork, second_fork);
			return (-1);
		}
	}
	if (stop(philosopher))
	{
		unlock_forks(first_fork, second_fork);
		return (-1);
	}
	return (0);
}
