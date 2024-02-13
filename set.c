/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:34:09 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/13 18:37:32 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	allocate_simulation_resources(t_simulation *simulation, int number_of_philosophers)
{

	if (number_of_philosophers <= 0)
		return (0); 
	simulation->philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
	if (!simulation->philosophers)
	{
		printf("Échec de l'allocation pour les philosophes.\n");
		return (0);
	}
	simulation->forks = malloc(sizeof(t_fork) * number_of_philosophers);
	if (!simulation->forks)
	{
		printf("Échec de l'allocation pour les fourchettes.\n");
		simulation->forks = NULL;
		return (0);
	}
	return (1);
}

int	initialize_mutexes(t_simulation *simulation)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&simulation->scheduler_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&simulation->death, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&simulation->write, NULL) != 0)
		return (0);
	while (i < simulation->params->number_of_philosophers)
	{
		if (pthread_mutex_init(&simulation->forks[i].mutex, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	initialize_philosopher(t_simulation *simulation, int i)
{
	simulation->philosophers[i].id = i + 1;
	simulation->philosophers[i].last_meal_time = current_timestamp_in_ms();
	simulation->philosophers[i].meals_eaten = 0;
	simulation->philosophers[i].is_dead = 0;
	simulation->philosophers[i].params = *(simulation->params);
	simulation->philosophers[i].simulation = simulation;
	simulation->philosophers[i].full = 0;
	simulation->philosophers[i].left_fork = &simulation->forks[i];
	if (simulation->params->number_of_philosophers == 1)
	{
		simulation->philosophers[i].right_fork = NULL;
	}
	else //if (simulation->params->number_of_philosophers % 2 == 0)
		simulation->philosophers[i].right_fork = &simulation->forks[(i + 1) % simulation->params->number_of_philosophers];
	// else 
	// 	simulation->philosophers[i].right_fork = &simulation->forks[(i - 1) % simulation->params->number_of_philosophers];	
	simulation->philosophers[i].monitor_launched = false;
	simulation->philosophers[i].thread_launched = false;
	if (pthread_mutex_init(&simulation->philosophers[i].mutex, NULL) != 0)
		return (0);
	return (1);
}

void	initialize_simulation_data(t_simulation *simulation, int number_of_philosophers)
{
	int	i;

	i = 0;
	if (!simulation || !simulation->philosophers || !simulation->forks)
		return ;
	if (!initialize_mutexes(simulation))
		return ;
	while (i < number_of_philosophers)
	{
		if (!initialize_philosopher(simulation, i))
			return ;
		i++;
	}
	simulation->full_philosophers = 0;
	simulation->stop = 0;

}


t_simulation	*init_simulation(t_simulation *simulation, int number_of_philosophers)
{
	if (!allocate_simulation_resources(simulation, number_of_philosophers))
	{
		printf("Failed to allocate ressources for simulation\n");
		free(simulation);
		return (NULL);
	}
	initialize_simulation_data(simulation, number_of_philosophers);
	return (simulation);
}

