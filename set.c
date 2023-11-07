/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:34:09 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/07 19:27:32 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int allocate_simulation_resources(t_simulation *simulation, int number_of_philosophers)
{
    printf("Allocation des ressources pour %d philosophes.\n", number_of_philosophers);
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
        free(simulation->philosophers);
        simulation->philosophers = NULL;
        return (0);
    }
    printf("Allocation des ressources réussie.\n");
    return (1);
}

void initialize_simulation_data(t_simulation *simulation, int number_of_philosophers)
{
    if (!simulation || !simulation->philosophers || !simulation->forks) {
        fprintf(stderr, "Error: simulation, philosophers, or forks are not initialized.\n");
        exit(EXIT_FAILURE);
    }
    printf("Initialisation des données pour la simulation.\n");

    if (pthread_mutex_init(&simulation->scheduler_mutex, NULL) != 0) {
        fprintf(stderr, "Error: Unable to initialize scheduler_mutex.\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while ( i < number_of_philosophers) 
    {
        simulation->philosophers[i].id = i + 1;
        printf("Initialisation du philosophe %d.\n", i + 1);
        simulation->philosophers[i].last_meal_time = current_timestamp_in_ms();
        simulation->philosophers[i].meals_eaten = 0;
        simulation->philosophers[i].is_dead = 0; // Assuming 'is_dead' is declared in 't_philosopher'
        simulation->philosophers[i].params = simulation->params; // Assuming you want to copy the parameters to each philosopher
        simulation->philosophers[i].simulation = simulation;
        simulation->philosophers[i].full = 0;
        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1)% number_of_philosophers];
        // Initialize the mutex for each philosopher
        if (pthread_mutex_init(&simulation->philosophers[i].mutex, NULL) != 0)
        {
            printf("Error: Unable to initialize mutex for philosopher %d.\n", i + 1);
            exit(EXIT_FAILURE);
        }

        // Initialize fork mutex for each philosopher.
        if (pthread_mutex_init(&simulation->forks[i].mutex, NULL) != 0) {
            printf("Error: Unable to initialize fork mutex for philosopher %d.\n", i);
            // Clean up philosopher mutexes and forks initialized up to this point before returning.
            for (int j = 0; j <= i; ++j) {
                pthread_mutex_destroy(&simulation->philosophers[j].mutex);
                if (j < i) {
                    pthread_mutex_destroy(&simulation->forks[j].mutex);
                }
            }
            pthread_mutex_destroy(&simulation->scheduler_mutex);
            return; // Or handle the error as appropriate.
        }
        i++;
    }

    printf("Données initialisées pour tous les philosophes.\n");
}


t_simulation* init_simulation(t_simulation *simulation, int number_of_philosophers)
{
    if (!allocate_simulation_resources(simulation, number_of_philosophers))
    {
        printf("Failed to allocate ressources for simulation\n");
        return NULL;
    }
    initialize_simulation_data(simulation, number_of_philosophers);
    return (simulation);
}

void free_simulation(t_simulation *simulation)
{
    int i;
    
    printf("Libération des ressources de simulation.\n");
    if (simulation)
    {
        // join_philosopher_threads(simulation);
        if (simulation->philosophers)
        {
            i = 0;
            printf("if 2");
            while (i < simulation->params.number_of_philosophers)
            {
                pthread_mutex_destroy(&simulation->philosophers[i].mutex);
                // pthread_mutex_destroy(&simulation->philosophers[i].eating_mutex);
                i++;
            }
            free(simulation->philosophers);
            simulation->philosophers = NULL;
        } 
        if (simulation->forks)
        {
            i = 0;
            printf("if 1");
            while (i < simulation->params.number_of_philosophers)
            {
                pthread_mutex_destroy(&simulation->forks[i].mutex);
                i++;
            }
            free(simulation->forks);
            simulation->forks = NULL;
        }
        pthread_mutex_destroy(&simulation->scheduler_mutex);
        printf("Ressources libérées pour la simulation.\n");
    }
    
    // pthread_mutex_destroy(&simulation->start_barrier);
    // free(simulation);
}
