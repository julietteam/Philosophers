/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:28:32 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 18:08:25 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_second_forks(t_philosopher *philosopher, \
pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (!stop(philosopher))
		pthread_mutex_lock(second_fork);
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	if (!stop(philosopher))
	{
		if (display_log(philosopher->simulation, \
		philosopher->id, "has taken a fork", philosopher) == -1)
		{	
			pthread_mutex_unlock(second_fork);
			pthread_mutex_unlock(first_fork);
			return (-1);
		}
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
	if (!stop(philosopher))
		pthread_mutex_lock(first_fork);
	if (stop(philosopher))
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (!stop(philosopher))
	{
		if (display_log(philosopher->simulation, \
		philosopher->id, "has taken a fork", philosopher) == -1)
		{	
			pthread_mutex_unlock(first_fork);
			return (-1);
		}
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
