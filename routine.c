/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2023/10/26 18:00:18 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool try_take_fork(pthread_mutex_t *fork)
{
    if (pthread_mutex_trylock(fork) == 0)
    {
        return true;
    }
    return false;
}
void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    // Initialisation des fourchettes basée sur l'ID du philosophe
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

    while (1)
    {
        if (current_timestamp_in_ms() - philosopher->last_meal_time > philosopher->params.time_to_die)
        {
            display_log(philosopher->id, "died");
            pthread_exit(NULL);  // Terminer le thread du philosophe
        }

        display_log(philosopher->id, "is thinking");
        usleep(10000); 

        // Vérification de l'ordonnanceur pour s'assurer que c'est le tour de ce philosophe de manger
        pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        while (philosopher->id != philosopher->simulation->current_philosopher_id)
        {
            pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
            usleep(5000);  // Attendre un peu avant de vérifier à nouveau
            pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        }

        // Prendre les fourchettes et manger
        pthread_mutex_lock(first_fork);
        display_log(philosopher->id, "has taken a fork");
        usleep(1000);
        pthread_mutex_lock(second_fork);
        display_log(philosopher->id, "has taken a fork");

        display_log(philosopher->id, "is eating");
        philosopher->last_meal_time = current_timestamp_in_ms();
        usleep(philosopher->params.time_to_eat);

        // Mettre à jour l'ordonnanceur pour le prochain philosophe
        philosopher->simulation->current_philosopher_id = (philosopher->id % philosopher->params.number_of_philosophers) + 1;
        pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);

        // Reposer les fourchettes
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);

        // Dormir
        display_log(philosopher->id, "is sleeping");
        usleep(philosopher->params.time_to_sleep);
    }
    return (NULL);
}

// void *philosopher_routine(void *arg)
// {
//     t_philosopher *philosopher = (t_philosopher *)arg;
//     pthread_mutex_t *first_fork;
//     pthread_mutex_t *second_fork;
    
    
//     // Si le philosophe a un ID impair, il prend d'abord la fourchette de gauche, sinon il prend d'abord la fourchette de droite.
//     if (philosopher->id % 2 == 1)
//     {
//         first_fork = &philosopher->left_fork->mutex;
//         second_fork = &philosopher->right_fork->mutex;
//     }
//     else
//     {
//         first_fork = &philosopher->right_fork->mutex;
//         second_fork = &philosopher->left_fork->mutex;
//     }
//     while (1) // Cette boucle continue tant que le philosophe n'est pas mort ou que d'autres conditions d'arrêt ne sont pas satisfaites
//     {
        
//         if (current_timestamp_in_ms() - philosopher->last_meal_time > philosopher->params.time_to_die)
//         {
//             display_log(philosopher->id, "died");
//             pthread_exit(NULL);  // Terminer le thread du philosophe
//         }
    
//         // Penser
//         display_log(philosopher->id, "is thinking");
//         usleep(10000);
//         // ... Ajoutez peut-être une certaine latence ici pour simuler le temps de réflexion.

//         // Prendre les fourchettes
//         pthread_mutex_lock(first_fork);
//         display_log(philosopher->id, "has taken a fork");
//         usleep(1000); 
//         pthread_mutex_lock(second_fork);
//         display_log(philosopher->id, "has taken a fork");

//         // Manger
//         display_log(philosopher->id, "is eating");
//         philosopher->last_meal_time = current_timestamp_in_ms();  // Mettre à jour le temps du dernier repas
//         usleep(philosopher->params.time_to_eat);
//         // ... Ici, vous pouvez utiliser usleep ou d'autres fonctions pour simuler le temps que le philosophe passe à manger.

//         // Reposer les fourchettes
//         pthread_mutex_unlock(first_fork);
//         pthread_mutex_unlock(second_fork);

//         // Dormir
//         display_log(philosopher->id, "is sleeping");
//         usleep(philosopher->params.time_to_sleep);
//         // ... Ajoutez une latence ici pour simuler le temps de sommeil.
//     }
//     return (NULL);
// }
void *monitor_philosopher(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *) arg;
    
    while (true) 
    {
        // Vérifiez si le philosophe est mort
        if (philosopher->is_dead) 
        {
            printf("philo %d is dead!\n", philosopher->id);
            pthread_exit(NULL);
        }
        usleep(1000); // Faites une pause de 1ms pour éviter une utilisation excessive du CPU
    }
}

void start_philosopher_threads(t_simulation *simulation)
{
    int i;

    i = 0;
    // Créez des threads pour chaque philosophe
    while (i < simulation->params.number_of_philosophers)
    {
        if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
        {
            printf("Error creating thread\n");
            exit(1);
        }
        if (pthread_create(&simulation->philosophers[i].thread, NULL, monitor_philosopher, &simulation->philosophers[i]) != 0)
        {
            printf("Error creating monitor thread\n");
            exit(1);
        }
        i++;
    }
    i = 0;
    while (i < simulation->params.number_of_philosophers)
    {
        pthread_join(simulation->philosophers[i].thread, NULL);
        i++;
    }
}

// // Tentative de prendre les fourchettes
//         if (try_take_fork(first_fork))
//         {
//             display_log(philosopher->id, "has taken a fork");
//             usleep(1000);
            
//             if (try_take_fork(second_fork))
//             {
//                 display_log(philosopher->id, "has taken a fork");
                
//                 // Manger
//                 display_log(philosopher->id, "is eating");
//                 philosopher->last_meal_time = current_timestamp_in_ms();
//                 usleep(philosopher->params.time_to_eat);

//                 // Reposer les fourchettes
//                 pthread_mutex_unlock(second_fork);
//             }
            
//             pthread_mutex_unlock(first_fork);
//         }