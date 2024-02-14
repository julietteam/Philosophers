/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 13:40:13 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/14 17:43:35 by juandrie         ###   ########.fr       */
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




