/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/15 18:44:54 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t log_mutex;

int main(int ac, char **av)
{
    t_simulation    simulation;
    int i;

    pthread_mutex_init(&log_mutex, NULL);
    if (ac < 5)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", av[0]);
        return (1);
    }
    simulation.params = malloc(sizeof(*(simulation.params)));
    if (simulation.params == NULL) 
    {
        printf("Memory allocation failed for simulation parameters.\n");
        return EXIT_FAILURE;
    }
    memset(simulation.params, 0, sizeof(*(simulation.params)));
    simulation.params->number_of_philosophers = atoi(av[1]); 
    simulation.params->time_to_die = atoi(av[2]);
    simulation.params->time_to_eat = atoi(av[3]);
    simulation.params->time_to_sleep = atoi(av[4]);
    
    if (ac == 6)
    {
        simulation.params->number_of_times_each_philosopher_must_eat = atoi(av[5]);
    }
    simulation.start_time = current_timestamp_in_ms(); 
    if (!init_simulation(&simulation, simulation.params->number_of_philosophers))
    {
        printf("Failed to initialize simulation\n");
        free(simulation.params);
        pthread_mutex_destroy(&log_mutex);
        return EXIT_FAILURE;
    }
    start_philosopher_threads(&simulation);
    i = 0;
    while (i < simulation.params->number_of_philosophers) 
    {
        // if (simulation.philosophers[i].thread_launched) 
        // {
            printf("rejoindre le thread du philo %d\n", i);
            pthread_join(simulation.philosophers[i].thread, NULL);
        //     simulation.philosophers[i].thread_launched = false;
        // }
        // if (simulation.philosophers[i].monitor_launched) 
        // {
            printf("thread du philo %d rejoint\n", i);
            pthread_join(simulation.philosophers[i].monitor_thread, NULL);
        //     simulation.philosophers[i].monitor_launched = false;
        // }
        i++;
    }
    // bool allThreadsJoined = false;
    // while (!allThreadsJoined) 
    // {
    //     pthread_mutex_lock(&simulation.scheduler_mutex);
    //     if (!simulation.is_running) 
    //     {
    //         printf("Fin de la simulation\n");
    //         for (int i = 0; i < simulation.params->number_of_philosophers; i++) {
    //         pthread_join(simulation.philosophers[i].thread, NULL);
    //         pthread_join(simulation.philosophers[i].monitor_thread, NULL);
    //     }
    //     allThreadsJoined = true;
    // }
    // pthread_mutex_unlock(&simulation.scheduler_mutex);
    // }

        // Vérification de l'état is_running
    pthread_mutex_lock(&simulation.scheduler_mutex);
    if (!simulation.is_running)
        printf("Fin de la simulation\n");
    pthread_mutex_unlock(&simulation.scheduler_mutex);
    free_simulation(&simulation);
    free(simulation.params);
    pthread_mutex_destroy(&log_mutex);
    return (0);
}
