/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2023/10/25 18:42:08 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
    t_simulation simulation;
    
    if (ac < 5 || ac > 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", av[0]);
        return (1);
    }
    // Initialisez la simulation
    simulation.params.number_of_philosophers = atoi(av[1]); // attention changer en ft_atoi
    simulation.params.time_to_die = atoi(av[2]);
    simulation.params.time_to_eat = atoi(av[3]);
    simulation.params.time_to_sleep = atoi(av[4]);

    if (ac == 6)
    {
        simulation.params.number_of_times_each_philosopher_must_eat = atoi(av[5]);
        simulation.params.is_times_to_eat_specified = true;
    } 
    else 
    {
        simulation.params.is_times_to_eat_specified = false;
    }
    // Lancez les threads pour chaque philosophe
    start_philosopher_threads(&simulation);

    // Ici, vous pourriez ajouter du code pour nettoyer la mémoire allouée, etc.

    return 0;
}
