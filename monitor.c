/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:09:48 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 18:12:04 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	create_philosopher_thread(t_simulation *simulation, int i)
{
	if (pthread_create(&simulation->philosophers[i].thread, NULL, \
	philosopher_routine, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].thread_launched = false;
		return (false);
	}
	simulation->philosophers[i].thread_launched = true;
	return (true);
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
	if (timed_out && !stop(philosopher))
	{
		is_dead(philosopher);
		status_simulation(philosopher);
		return (-1);
	}
	if (end(philosopher) == -1)
		return (-1);
	return (0);
}

int	monitor_cycle(t_simulation	*simulation)
{
	int				i;
	int				full;

	i = 0;
	while (i < simulation->params->number_of_philosophers && \
	!stop(simulation->philosophers))
	{
		if (stop(simulation->philosophers))
			return (-1);
		if (check_philosopher_status(&simulation->philosophers[i]) == -1)
			break ;
		i++;
	}
	if (!stop(simulation->philosophers))
	{
		pthread_mutex_lock(&simulation->scheduler_mutex);
		full = simulation->full_philosophers;
		pthread_mutex_unlock(&simulation->scheduler_mutex);
		if (full)
			return (-1);
		usleep(50);
		if (stop(simulation->philosophers))
			return (-1);
	}
	return (0);
}

void	*global_monitor(void *arg)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)arg;
	while (!stop(simulation->philosophers))
	{
		if (stop(simulation->philosophers))
			return (NULL);
		if (monitor_cycle(simulation) == -1)
			return (NULL);
	}
	return (NULL);
}

int	start_philosopher_threads(t_simulation *simulation)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < simulation->params->number_of_philosophers)
	{
		if (!create_philosopher_thread(simulation, i))
			return (-1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, global_monitor, simulation) != 0)
		return (-1);
	if (pthread_join(monitor_thread, NULL) != 0)
		return (-1);
	return (0);
}
