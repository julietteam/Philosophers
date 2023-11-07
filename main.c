/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/07 19:34:35 by juandrie         ###   ########.fr       */
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
    memset(&simulation, 0, sizeof(t_simulation));
    simulation.params.number_of_philosophers = atoi(av[1]); // attention changer en ft_atoi
    simulation.params.time_to_die = atoi(av[2]);
    simulation.params.time_to_eat = atoi(av[3]);
    simulation.params.time_to_sleep = atoi(av[4]);
    if (ac > 6)
    {
        simulation.params.number_of_times_each_philosopher_must_eat = atoi(av[5]);
    }
    init_simulation(&simulation, simulation.params.number_of_philosophers);
   
    start_philosopher_threads(&simulation);

    free_simulation(&simulation);
    pthread_mutex_destroy(&log_mutex);
    return (0);
}
