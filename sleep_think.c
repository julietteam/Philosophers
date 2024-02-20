/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_think.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:40:31 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 18:05:13 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	remaining_to_sleep(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
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
			remaining = philosopher->params.time_to_sleep - elapsed;
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
			if (stop(philosopher))
				return (-1);
		}
	}
	return (0);
}

int	to_sleep(t_philosopher *philosopher)
{
	if (!stop(philosopher))
	{
		if (display_log(philosopher->simulation, \
		philosopher->id, "is sleeping", philosopher) == -1)
			return (-1);
	}
	if (remaining_to_sleep(philosopher) == -1)
		return (-1);
	return (0);
}

int	think(t_philosopher *philosopher)
{
	if (!stop(philosopher))
	{
		if (display_log(philosopher->simulation, \
		philosopher->id, "is thinking", philosopher) == -1)
			return (-1);
	}
	return (0);
}
