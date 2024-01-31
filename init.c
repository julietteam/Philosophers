/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:31:46 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/31 15:30:22 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_simulation(t_simulation *simulation, int ac, char **av)
{
	simulation->params = malloc(sizeof(*(simulation->params)));
	if (simulation->params == NULL)
	{
		printf("Memory allocation failed for simulation parameters.\n");
		return (0);
	}
	memset(simulation->params, 0, sizeof(*(simulation->params)));
	simulation->params->number_of_philosophers = atoi(av[1]);
	simulation->params->time_to_die = atoi(av[2]);
	simulation->params->time_to_eat = atoi(av[3]);
	simulation->params->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		simulation->params->number_of_times_each_philosopher_must_eat = atoi(av[5]);
	simulation->start_time = current_timestamp_in_ms();
	if (!init_simulation(simulation, simulation->params->number_of_philosophers))
	{
		printf("Failed to initialize simulation\n");
		free(simulation->params);
		return (0);
	}
	return (1);
}

void	start_simulation(t_simulation *simulation)
{
	start_philosopher_threads(simulation);
}

void	join_philosopher_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->params->number_of_philosophers)
	{
		//printf("Rejoindre le thread du philo %d\n", i);
		pthread_join(simulation->philosophers[i].thread, NULL);
		//printf("Thread du philo %d rejoint\n", i);
		pthread_join(simulation->philosophers[i].monitor_thread, NULL);
		i++;
	}
}

void	finalize_simulation(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->scheduler_mutex);
	// if (!simulation->is_running)
	// 	printf("Fin de la simulation\n");
	pthread_mutex_unlock(&simulation->scheduler_mutex);
	free_simulation(simulation);
	free(simulation->params);
}
