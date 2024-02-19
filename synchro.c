/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:35:47 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/19 18:04:46 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	before_departure(t_philosopher *philosopher)
{
	long long	start_delay;

	start_delay = philosopher->simulation->params->number_of_philosophers;
	while (!start_delay)
		usleep(100);
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", \
	philosopher) == -1)
		return (-1);
	if ((philosopher->simulation->params->number_of_philosophers % 2 == 0 \
		&& philosopher->id % 2 != 0) 
		|| (philosopher->simulation->params->number_of_philosophers % 2 != 0 \
		&& philosopher->id % 2 == 0))
	{
		usleep(philosopher->simulation->params->time_to_eat * 100 / 2);
	}
	return (0);
}

int	wait_after_thinking(long long delay, t_philosopher *philosopher)
{
	long long	time_now;

	time_now = current_timestamp_in_ms();
	while (current_timestamp_in_ms() - time_now < delay)
	{
		usleep(50);
		if (stop(philosopher))
			return (-1);
	}
	return (0);
}

void	set_sync(int i, t_simulation *simulation)
{
	int	nb;

	nb = simulation->params->number_of_philosophers;
	if (nb == 3)
	{
		if (i % 2 == 0)
			simulation->philosophers[i].sync = 50;
		else
			simulation->philosophers[i].sync = 100;
	}
	set_sync_2(nb, i, simulation->philosophers);
}

void	set_sync_2(int nb, int i, t_philosopher *philosopher)
{
	if (nb % 2 != 0)
	{
		if ((philosopher[i].simulation->params->time_to_eat * 3 \
		<= philosopher[i].simulation->params->time_to_die) \
		&& philosopher[i].simulation->params->time_to_sleep \
		< philosopher[i].simulation->params->time_to_eat)
		{
			(philosopher[i].sync += \
			philosopher[i].simulation->params->time_to_eat \
			- philosopher[i].simulation->params->time_to_sleep);
		}
		else if (philosopher[i].simulation->params->time_to_eat * 3 > \
		philosopher[i].simulation->params->time_to_die \
		&& philosopher[i].simulation->params->time_to_sleep \
		+ philosopher[i].simulation->params->time_to_eat \
		< philosopher[i].simulation->params->time_to_die)
		{
			(philosopher[i].sync += \
			philosopher[i].simulation->params->time_to_die \
			- philosopher[i].simulation->params->time_to_eat \
			+ philosopher[i].simulation->params->time_to_sleep);
		}
	}
}

int	stop(t_philosopher *philosopher)
{
	int			stop;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (stop);
}

int	dead(t_philosopher *philosopher)
{
	int	dead;

	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	return (dead);
}
