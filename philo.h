/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:27:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/10/26 17:56:35 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

// #define TIME_TO_DIE 100  // Le philosophe meurt s'il n'a pas mangé pendant 100 ms

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_fork
{
    pthread_mutex_t mutex;
}               t_fork;

typedef struct  s_params
{
    int     number_of_philosophers; 
    int     time_to_die; 
    int     time_to_eat; 
    int     time_to_sleep; 
    int     number_of_times_each_philosopher_must_eat;
    bool    is_times_to_eat_specified;
}               t_params;

typedef struct s_philosopher
{
    int id;
    int is_dead;
    long last_meal_time;
    int meals_eaten;
    t_fork *left_fork;
    t_fork *right_fork;
    pthread_t thread;
    t_params params;
    struct s_simulation *simulation;  // Ajouter cette ligne
}               t_philosopher;

typedef struct s_simulation
{
    t_params params;
    // int number_of_philosophers;
    t_philosopher *philosophers;
    t_fork *forks;
    pthread_mutex_t scheduler_mutex; // Mutex pour l'ordonnanceur
    int current_philosopher_id;  
}               t_simulation;

t_simulation* init_simulation(t_simulation *simulation, int number_of_philosophers);
void free_simulation(t_simulation *simulation);
void *philosopher_routine(void *arg);
void start_philosopher_threads(t_simulation *simulation);
long long current_timestamp_in_ms();
void display_log(int philosopher_id, const char *action);

#endif // PHILO_H