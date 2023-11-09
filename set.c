/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:34:09 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/09 14:49:41 by juandrie         ###   ########.fr       */
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
        free(simulation->philosophers);
        simulation->philosophers = NULL;
        return (0);
    }
    simulation->forks = malloc(sizeof(t_fork) * number_of_philosophers);
    if (!simulation->forks)
    {
        printf("Échec de l'allocation pour les fourchettes.\n");
        free(simulation->forks);
        simulation->forks = NULL;
        return (0);
    }
    printf("Allocation des ressources réussie.\n");
    return (1);
}

void initialize_simulation_data(t_simulation *simulation, int number_of_philosophers)
{
    if (!simulation || !simulation->philosophers || !simulation->forks)
        exit(EXIT_FAILURE);
    printf("Initialisation des données pour la simulation.\n");

    if (pthread_mutex_init(&simulation->scheduler_mutex, NULL) != 0) 
        exit(EXIT_FAILURE);
    int i = 0;
    while ( i < number_of_philosophers) 
    {
        simulation->philosophers[i].id = i + 1;
        printf("Initialisation du philosophe %d.\n", i + 1);
        simulation->philosophers[i].last_meal_time = current_timestamp_in_ms();
        simulation->philosophers[i].meals_eaten = 0;
        simulation->philosophers[i].is_dead = 0; // Assuming 'is_dead' is declared in 't_philosopher'
        simulation->philosophers[i].params = *(simulation->params); // Assuming you want to copy the parameters to each philosopher
        simulation->philosophers[i].simulation = simulation;
        simulation->philosophers[i].full = 0;
        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1)% number_of_philosophers];
        if (pthread_mutex_init(&simulation->philosophers[i].mutex, NULL) != 0)
            exit(EXIT_FAILURE);
        if (pthread_mutex_init(&simulation->forks[i].mutex, NULL) != 0) 
        {
            int j = 0;
            while (j <= i) 
            {
                pthread_mutex_destroy(&simulation->philosophers[j].mutex);
                if (j < i) 
                    pthread_mutex_destroy(&simulation->forks[j].mutex);
                ++j;
            }
            pthread_mutex_destroy(&simulation->scheduler_mutex);
            return;
       
        }
        if (pthread_mutex_init(&simulation->philosophers[i].eating_mutex, NULL) != 0)
            exit(EXIT_FAILURE);
        i++;
    }
    simulation->full_philosophers = 0;
    simulation->is_running = 1;
    printf("Données initialisées pour tous les philosophes.\n");
}


t_simulation* init_simulation(t_simulation *simulation, int number_of_philosophers)
{
    if (!allocate_simulation_resources(simulation, number_of_philosophers))
    {
        printf("Failed to allocate ressources for simulation\n");
        free(simulation);
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
        if (simulation->philosophers)
        {
            i = 0;
            printf("if 1\n");
            while (i < simulation->params->number_of_philosophers)
            {
                pthread_mutex_destroy(&simulation->philosophers[i].mutex);
                pthread_mutex_destroy(&simulation->philosophers[i].eating_mutex);
                i++;
            }
            free(simulation->philosophers);
            simulation->philosophers = NULL;
        } 
        if (simulation->forks)
        {
            i = 0;
            printf("if 2\n");
            while (i < simulation->params->number_of_philosophers)
            {
                pthread_mutex_destroy(&simulation->forks[i].mutex);
                i++;
            }
            free(simulation->forks);
            simulation->forks = NULL;
        }
        pthread_mutex_destroy(&simulation->scheduler_mutex);
        // pthread_mutex_destroy(&simulation->start_barrier);
        printf("Ressources libérées pour la simulation.\n");
    }
    
}
