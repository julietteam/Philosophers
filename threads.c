/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:07 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/02 17:34:50 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


// void *philosopher_routine(void *arg)
// {
//     t_philosopher *philosopher = (t_philosopher *)arg;
//     philosopher->last_meal_time = current_timestamp_in_ms(); // Initialisez last_meal_time avec le temps actuel
//     philosopher->is_dead = 0; // Le philosophe commence vivant
//     pthread_mutex_t *first_fork;
//     pthread_mutex_t *second_fork;

//     // Initialisation des fourchettes basée sur l'ID du philosophe
//     if (philosopher->id % 2 == 0)
//     {
//         first_fork = &philosopher->left_fork->mutex;
//         second_fork = &philosopher->right_fork->mutex;
//     }
//     else
//     {
//         first_fork = &philosopher->right_fork->mutex;
//         second_fork = &philosopher->left_fork->mutex;
//     }

//     while (1)
//     {
//         if (current_timestamp_in_ms() - philosopher->last_meal_time > philosopher->params.time_to_die)
//         {
//             display_log(philosopher->id, "died");
//             pthread_exit(NULL);  // Terminer le thread du philosophe
//         }

//         display_log(philosopher->id, "is thinking");
//         usleep(10000); 

//         // Vérification de l'ordonnanceur pour s'assurer que c'est le tour de ce philosophe de manger
//         pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
//         while (philosopher->id != philosopher->simulation->current_philosopher_id)
//         {
//             pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
//             usleep(5000);  // Attendre un peu avant de vérifier à nouveau
//             pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
//         }

//         // Prendre les fourchettes et manger
//         pthread_mutex_lock(first_fork);
//         display_log(philosopher->id, "has taken a fork");
//         usleep(1000);
//         pthread_mutex_lock(second_fork);
//         display_log(philosopher->id, "has taken a fork");

//         display_log(philosopher->id, "is eating");
//         philosopher->last_meal_time = current_timestamp_in_ms();
//         usleep(philosopher->params.time_to_eat);

//         // Mettre à jour l'ordonnanceur pour le prochain philosophe
//         philosopher->simulation->current_philosopher_id = (philosopher->id % philosopher->params.number_of_philosophers) + 1;
//         pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);

//         // Reposer les fourchettes
//         pthread_mutex_unlock(second_fork);
//         pthread_mutex_unlock(first_fork);

//         // Dormir
//         display_log(philosopher->id, "is sleeping");
//         usleep(philosopher->params.time_to_sleep);
//     }
//     return (NULL);
// }