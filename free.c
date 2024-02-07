/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:40:13 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/07 13:58:09 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_philosopher_mutexes(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->params->number_of_philosophers)
	{
		pthread_mutex_destroy(&simulation->philosophers[i].mutex);
		//pthread_mutex_destroy(&simulation->philosophers[i].eating_mutex);
		i++;
	}
}

void	free_philosophers(t_simulation *simulation)
{
	if (simulation->philosophers)
	{
		free(simulation->philosophers);
		simulation->philosophers = NULL;
	}
}

void	destroy_fork_mutexes(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->params->number_of_philosophers)
	{
		pthread_mutex_destroy(&simulation->forks[i].mutex);
		i++;
	}
}

void	free_forks(t_simulation *simulation)
{
	if (simulation->forks)
	{
		free(simulation->forks);
		simulation->forks = NULL;
	}
}

void	free_simulation(t_simulation *simulation)
{
	//printf("Libération des ressources de simulation.\n");
	if (simulation)
	{
		destroy_philosopher_mutexes(simulation);
		free_philosophers(simulation);
		destroy_fork_mutexes(simulation);
		free_forks(simulation);
		pthread_mutex_destroy(&simulation->scheduler_mutex);
		pthread_mutex_destroy(&simulation->death);
		pthread_mutex_destroy(&simulation->write);
		
	}
}

void	join_philosopher(t_simulation *simulation, int philosopher_count)
{
	int	i;

	i = 0;
	while (i < philosopher_count)
	{
		if (simulation->philosophers[i].thread_launched)
		{
			pthread_join(simulation->philosophers[i].thread, NULL);
			simulation->philosophers[i].thread_launched = false;
		}
		if (simulation->philosophers[i].monitor_launched)
		{
			pthread_join(simulation->philosophers[i].monitor_thread, NULL);
			simulation->philosophers[i].monitor_launched = false;
		}
		i++;
	}
}
void	free_resources(t_simulation *simulation)
{
	if (simulation->forks)
	{
		free(simulation->forks);
		simulation->forks = NULL;
	}
	if (simulation->philosophers)
	{
		free(simulation->philosophers);
		simulation->philosophers = NULL;
	}
}

void	exit_cleanly(t_simulation *simulation, int philosopher_count)
{
	//printf("Nettoyage et sortie après l'erreur.\n");
	join_philosopher(simulation, philosopher_count);
	free_resources(simulation);
	//printf("Sortie propre réalisée.\n");
	exit(1);
}


