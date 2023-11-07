/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:27:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/07 18:57:44 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

extern pthread_mutex_t log_mutex;

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
}               t_params;

typedef struct s_philosopher
{
    bool thread_launched;
    bool monitor_launched;
    int full;
    int id;
    int is_dead; // etat de vie du Philosophe : 1 si mort, 0 si vivant
    long long last_meal_time;
    int meals_eaten;
    t_fork *left_fork;
    t_fork *right_fork;
    pthread_t thread;
    pthread_t monitor_thread;
    t_params params;
    struct s_simulation *simulation;
    pthread_mutex_t mutex;
    pthread_mutex_t eating_mutex;
}               t_philosopher;

typedef struct s_simulation
{
    t_params params;
    t_philosopher *philosophers;
    t_fork *forks;
    pthread_mutex_t scheduler_mutex; // Mutex pour l'ordonnanceur
    int current_philosopher_id; 
    int is_running;
    int total_meals_eaten;
    int full_philosophers;
    pthread_mutex_t start_barrier;
}               t_simulation;


t_simulation* init_simulation(t_simulation *simulation, int number_of_philosophers);
void free_simulation(t_simulation *simulation);
void *philosopher_routine(void *arg);
void start_philosopher_threads(t_simulation *simulation);
long long current_timestamp_in_ms();
void display_log(int philosopher_id, const char *action);

#endif // PHILO_H