/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 17:55:42 by juandrie         ###   ########.fr       */
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

int	dead(t_philosopher *philosopher)
{
	int	dead;

	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	return (dead);
}
