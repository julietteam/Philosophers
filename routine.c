/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2023/10/25 19:00:52 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_simulation *simulation = philosopher->simulation;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
    int time_to_die = simulation->params.time_to_die;
    
    // Si le philosophe a un ID impair, il prend d'abord la fourchette de gauche, sinon il prend d'abord la fourchette de droite.
    if (philosopher->id % 2 == 1)
    {
        first_fork = &philosopher->left_fork->mutex;
        second_fork = &philosopher->right_fork->mutex;
    }
    else
    {
        first_fork = &philosopher->right_fork->mutex;
        second_fork = &philosopher->left_fork->mutex;
    }
    while (1) // Cette boucle continue tant que le philosophe n'est pas mort ou que d'autres conditions d'arrêt ne sont pas satisfaites
    {
        
        if (current_timestamp_in_ms() - philosopher->last_meal_time > simulation->params.time_to_die)
        {
            display_log(philosopher->id, "died");
            pthread_exit(NULL);  // Terminer le thread du philosophe
        }
    
        // Penser
        display_log(philosopher->id, "is thinking");
        usleep(100);
        // ... Ajoutez peut-être une certaine latence ici pour simuler le temps de réflexion.

        // Prendre les fourchettes
        pthread_mutex_lock(first_fork);
        display_log(philosopher->id, "has taken a fork");
        usleep(100); 
        pthread_mutex_lock(second_fork);
        display_log(philosopher->id, "has taken a fork");

        // Manger
        display_log(philosopher->id, "is eating");
        philosopher->last_meal_time = current_timestamp_in_ms();  // Mettre à jour le temps du dernier repas
        usleep(simulation->params.time_to_eat);
        // ... Ici, vous pouvez utiliser usleep ou d'autres fonctions pour simuler le temps que le philosophe passe à manger.

        // Reposer les fourchettes
        pthread_mutex_unlock(first_fork);
        pthread_mutex_unlock(second_fork);

        // Dormir
        display_log(philosopher->id, "is sleeping");
        usleep(simulation->params.time_to_sleep);
        // ... Ajoutez une latence ici pour simuler le temps de sommeil.
    }

    return (NULL);
}

void start_philosopher_threads(t_simulation *simulation)
{
    int number_of_philosophers;
    int i;

    number_of_philosophers = simulation->number_of_philosophers;
    i = 0;
    // Créez des threads pour chaque philosophe
    while (i < number_of_philosophers)
    {
        if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
        {
            perror("Error creating thread");
            exit(1);
        }
        i++;
    }

    // Attendez que tous les threads se terminent 
    // (bien que, selon votre logique, ils pourraient ne jamais se terminer)
    i = 0;
    while (i < number_of_philosophers)
    {
        pthread_join(simulation->philosophers[i].thread, NULL);
        i++;
    }
}