/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:34:09 by juandrie          #+#    #+#             */
/*   Updated: 2023/10/25 17:53:11 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_simulation *init_simulation(int number_of_philosophers)
{
    t_simulation *simulation;
    int i;
    
    i = 0;
    simulation = malloc(sizeof(t_simulation));
    if (!simulation) 
        return (NULL);

    simulation->number_of_philosophers = number_of_philosophers;
    simulation->philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
    simulation->forks = malloc(sizeof(t_fork) * number_of_philosophers);

    if (!simulation->philosophers || !simulation->forks) 
        return (NULL);

    while(i < number_of_philosophers) 
    {
        simulation->philosophers[i].id = i + 1;
        simulation->philosophers[i].last_meal_time = current_timestamp_in_ms();  // Initialiser à l'heure actuelle
        simulation->philosophers[i].meals_eaten = 0;

        // Initialize mutex for each fork
        pthread_mutex_init(&simulation->forks[i].mutex, NULL);

        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1) % number_of_philosophers];
        i++;
    }
    return (simulation);
}

void free_simulation(t_simulation *simulation)
{
    if (simulation)
    {
        free(simulation->philosophers);
        free(simulation->forks);
        free(simulation);
    }
}
