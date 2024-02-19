/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/19 18:16:30 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	update_scheduler(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->params.number_of_times_each_philosopher_must_eat > 0 \
	&& philosopher->meals_eaten \
	>= philosopher->params.number_of_times_each_philosopher_must_eat)
	{
		philosopher->full = 1;
		philosopher->simulation->full_philosophers++;
		if (philosopher->simulation->full_philosophers \
		== philosopher->params.number_of_philosophers)
		{
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
			return (-1);
		}
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0);
}

int	is_dead(t_philosopher *philosopher)
{
	if (display_log(philosopher->simulation, philosopher->id, "died", \
	philosopher) == -1)
	{
		pthread_mutex_lock(&philosopher->simulation->death);
		philosopher->is_dead = 1;
		pthread_mutex_unlock(&philosopher->simulation->death);
		return (-1);
	}
	return (0);
}

void	status_simulation(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	philosopher->simulation->stop = 1;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
}

int	end(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->simulation->stop)
	{
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0);
}

int	check_philosopher_status(t_philosopher *philosopher)
{
	long long	time_since_last_meal;
	int			timed_out;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	time_since_last_meal = current_timestamp_in_ms() \
	- philosopher->last_meal_time;
	timed_out = time_since_last_meal > philosopher->params.time_to_die;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (timed_out && !dead(philosopher) && !stop(philosopher))
	{
		is_dead(philosopher);
		status_simulation(philosopher);
		return (-1);
	}
	if (end(philosopher) == -1)
		return (-1);
	return (0);
}
