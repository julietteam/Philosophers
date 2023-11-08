/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/08 17:57:41 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t log_mutex;

int main(int ac, char **av)
{
    t_simulation    simulation;

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
    init_simulation(&simulation, simulation.params->number_of_philosophers);
   
    start_philosopher_threads(&simulation);

    free_simulation(&simulation);
    free(simulation.params);
    pthread_mutex_destroy(&log_mutex);
    return (0);
}
