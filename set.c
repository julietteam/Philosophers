/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:34:09 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/02 18:02:16 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int allocate_simulation_resources(t_simulation *simulation, int number_of_philosophers)
{
    if (number_of_philosophers <= 0)
        return (0); 

    simulation->philosophers = malloc(sizeof(t_philosopher) * number_of_philosophers);
    simulation->forks = malloc(sizeof(t_fork) * number_of_philosophers);

    if (!simulation->philosophers || !simulation->forks)
    {
        free(simulation->philosophers);
        free(simulation->forks);
        return (0);
    }

    return (1);
}

void initialize_simulation_data(t_simulation *simulation, int number_of_philosophers)
{
    int i;

    i = 0;
    pthread_mutex_init(&simulation->scheduler_mutex, NULL);
    pthread_barrier_init(&simulation->start_barrier, NULL, number_of_philosophers);

    while (i < number_of_philosophers) 
    {
        simulation->philosophers[i].id = i + 1;
        simulation->philosophers[i].last_meal_time = current_timestamp_in_ms();
        simulation->philosophers[i].meals_eaten = 0;

        // Initialize mutex for each fork
        pthread_mutex_init(&simulation->forks[i].mutex, NULL);

        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1) % number_of_philosophers];
        i++;
    }

}

t_simulation* init_simulation(t_simulation *simulation, int number_of_philosophers)
{
    if (!allocate_simulation_resources(simulation, number_of_philosophers))
        return NULL;

    initialize_simulation_data(simulation, number_of_philosophers);

    return (simulation);
}

void free_simulation(t_simulation *simulation)
{
    int i;

    if (simulation)
    {
        if (simulation->forks)
        {
            i = 0;
            while (i < simulation->params.number_of_philosophers)
            {
                pthread_mutex_destroy(&simulation->forks[i].mutex);
                i++;
            }
            free(simulation->forks);
        }
        if (simulation->philosophers)
            free(simulation->philosophers);
    }
    pthread_mutex_destroy(&simulation->scheduler_mutex);
    pthread_barrier_destroy(&simulation->start_barrier);

}
