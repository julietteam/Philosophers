/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:09:48 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/19 23:53:35 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int	monitor_philosopher_cycle(t_philosopher *philosopher)
// {
// 	int	full;

// 	while (!dead(philosopher) || !stop(philosopher))
// 	{
// 		if (stop(philosopher) || dead(philosopher))
// 		{
// 			return (-1);
// 		}
// 		usleep(50);
// 		if (stop(philosopher) || dead(philosopher))
// 		{
// 			return (-1);
// 		}
// 		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
// 		full = philosopher->full;
// 		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
// 		if (full)
// 			return (-1);
// 		if (check_philosopher_status(philosopher) == -1)
// 			return (-1);
// 	}
// 	return (0);
// }

// void	*monitor_philosopher(void *arg)
// {
// 	t_philosopher	*philosopher;

// 	philosopher = (t_philosopher *)arg;
// 	while (!dead(philosopher) && !stop(philosopher))
// 	{
// 		if (monitor_philosopher_cycle(philosopher) == -1)
// 			break ;
// 	}
// 	return (NULL);
// }

bool	create_philosopher_thread(t_simulation *simulation, int i)
{
	if (pthread_create(&simulation->philosophers[i].thread, NULL, \
	philosopher_routine, &simulation->philosophers[i]) != 0)
	{
		simulation->philosophers[i].thread_launched = false;
		return (false);
	}
	simulation->philosophers[i].thread_launched = true;
	// if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, 
	// monitor_philosopher, &simulation->philosophers[i]) != 0)
	// {
	// 	simulation->philosophers[i].monitor_launched = false;
	// 	return (false);
	// }
	// simulation->philosophers[i].monitor_launched = true;
	return (true);
}

void	creation_error(t_simulation *simulation, int philosopher_count)
{
	int	j;

	j = 0;
	while (j < philosopher_count)
	{
		if (simulation->philosophers[j].thread_launched)
		{
			pthread_join(simulation->philosophers[j].thread, NULL);
		}
		// if (simulation->philosophers[j].monitor_launched)
		// {
		// 	pthread_join(simulation->philosophers[j].monitor_thread, NULL);
		// }
		j++;
	}
	free_simulation(simulation);
}

void	*global_monitor(void *arg)
{
	t_simulation	*simulation;
	int				i;

	simulation = (t_simulation *)arg;
	while (!stop(simulation->philosophers))
	{
		i = 0;
		usleep(50);
		while (i < simulation->params->number_of_philosophers && !stop(simulation->philosophers))
		{
			if (check_philosopher_status(&simulation->philosophers[i]) == -1)
			{
				break ;
			}
			i++;
		}
		pthread_mutex_lock(&simulation->scheduler_mutex);
 		int full = simulation->full_philosophers;
 		pthread_mutex_unlock(&simulation->scheduler_mutex); 	
		if (full)
			break ;
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

// int	start_philosopher_threads(t_simulation *simulation)
// {
// 	int		philosopher_count;
// 	int		i;
// 	bool	error_occurred;

// 	philosopher_count = simulation->params->number_of_philosophers;
// 	error_occurred = false;
// 	i = 0;
// 	while (i < philosopher_count)
// 	{
// 		if (!create_philosopher_thread(simulation, i))
// 		{
// 			error_occurred = true;
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (error_occurred)
// 	{
// 		creation_error(simulation, philosopher_count);
// 	}
// 	return (0);
// }
