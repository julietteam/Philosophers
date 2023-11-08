/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/08 19:06:41 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{ 
    if (!philosopher->simulation->is_running) 
        return;
        
    pthread_mutex_lock(first_fork);
    display_log(philosopher->simulation, philosopher->id, "has taken a fork");
    pthread_mutex_lock(second_fork);
    display_log(philosopher->simulation, philosopher->id, "has taken a fork");
}

void put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(second_fork);
}

void eat(t_philosopher *philosopher) 
{
    pthread_mutex_lock(&philosopher->eating_mutex);
    printf("%lld %d starts eating\n", current_timestamp_in_ms(), philosopher->id);
    philosopher->last_meal_time = current_timestamp_in_ms();
    display_log(philosopher->simulation, philosopher->id, "is eating");
    usleep(philosopher->params.time_to_eat * 500);
    philosopher->meals_eaten++; 
    printf("Philosopher %d has eaten %d times so far.\n", philosopher->id, philosopher->meals_eaten);
    pthread_mutex_unlock(&philosopher->eating_mutex);
}

void think_and_sleep(t_philosopher *philosopher)
{
    display_log(philosopher->simulation, philosopher->id, "is sleeping");
    usleep(philosopher->params.time_to_sleep * 1000);
    if (!philosopher->simulation->is_running) 
        return;
    display_log(philosopher->simulation, philosopher->id, "is thinking");
    usleep(1000);
}

void update_scheduler(t_philosopher *philosopher) 
{
    philosopher->simulation->full_philosophers = 0;
     
    pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        
    if (philosopher->meals_eaten >= philosopher->params.number_of_times_each_philosopher_must_eat)
    { 
        philosopher->full = 1;
        philosopher->simulation->full_philosophers++;
        if (philosopher->simulation->full_philosophers == philosopher->params.number_of_philosophers) 
        {
            philosopher->simulation->is_running = 0;
        }
    }
    pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
}

void *philosopher_routine(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    philosopher->is_dead = 0; 
    philosopher->simulation->is_running = 1;
    philosopher->full = 0;
    philosopher->meals_eaten = 0;
    philosopher->last_meal_time = current_timestamp_in_ms();

    pthread_mutex_t *first_fork, *second_fork;

    if (philosopher->id % 2 == 0) 
    { 
        first_fork = &philosopher->left_fork->mutex;
        second_fork = &philosopher->right_fork->mutex;
    } 
    else 
    { 
        first_fork = &philosopher->right_fork->mutex; 
        second_fork = &philosopher->left_fork->mutex;
    }

    while (!philosopher->is_dead && philosopher->simulation->is_running)
    {
        // if (!philosopher->simulation->is_running)
        //     break;
        // if (philosopher->full != 1)
        //     eat(philosopher);  
        take_forks(philosopher, first_fork, second_fork);
        if (!philosopher->simulation->is_running)
            break;
        if (philosopher->full != 1)
            eat(philosopher);
        if (philosopher->full == 1 || !philosopher->simulation->is_running)
            break; 
        if (!philosopher->simulation->is_running)
            break; 
        put_forks(first_fork, second_fork);
        if (!philosopher->simulation->is_running)
            break;
        think_and_sleep(philosopher);
        if (!philosopher->simulation->is_running)
            break;
        update_scheduler(philosopher);
        if (!philosopher->simulation->is_running)
            break; //
    }
    return (NULL);
}


void *monitor_philosopher(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    philosopher->last_meal_time = current_timestamp_in_ms();
    philosopher->is_dead = 0; // Ensure this philosopher is marked as alive to start
    philosopher->simulation->is_running = 1;
    
    while (philosopher->simulation->is_running) 
    {
        usleep(5000); 
        pthread_mutex_lock(&philosopher->eating_mutex);
        if (current_timestamp_in_ms() - philosopher->last_meal_time > philosopher->params.time_to_die) 
        {
            philosopher->is_dead = 1;
            display_log(philosopher->simulation, philosopher->id, "died");
            philosopher->simulation->is_running = 0;
            pthread_mutex_unlock(&philosopher->eating_mutex);
            break; 
        }
        pthread_mutex_unlock(&philosopher->eating_mutex);
    }
    return (NULL);
}


void exit_cleanly(t_simulation *simulation, int philosopher_count)
{
    int i;

    i = 0;
    printf("Nettoyage et sortie après l'erreur.\n"); 
    while(i < philosopher_count)
    {
        if(simulation->philosophers[i].thread_launched)
        {
            pthread_join(simulation->philosophers[i].thread, NULL);
            simulation->philosophers[i].thread_launched = false;
        }
        if(simulation->philosophers[i].monitor_launched)
        {
            pthread_join(simulation->philosophers[i].monitor_thread, NULL);
            simulation->philosophers[i].monitor_launched = false;
        }
        i++;
    }
    if(simulation->forks)
    {
        free(simulation->forks);
        simulation->forks = NULL;
    }
    if (simulation->philosophers)
    {
        free(simulation->philosophers);
        simulation->philosophers = NULL;
    }
    printf("Sortie propre réalisée.\n");
    exit (EXIT_FAILURE);
}

void start_philosopher_threads(t_simulation *simulation)
{
    int i;
    int philosopher_count;
    
    i = 0;
    printf("Démarrage des threads des philosophes.\n");
    philosopher_count = simulation->params->number_of_philosophers;
    while (i < philosopher_count)
    {
        if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) == 0)
        {
            simulation->philosophers[i].thread_launched = true;
            printf("Thread pour philosophe %d lancé.\n", i);
        }
        else
            exit_cleanly(simulation, i);
        if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, monitor_philosopher, &simulation->philosophers[i]) == 0)
            simulation->philosophers[i].monitor_launched = true;
        else
            exit_cleanly(simulation, i);
        i++;
    }
    i = 0;
    while (i < simulation->params->number_of_philosophers)
    {
        if(simulation->philosophers[i].thread_launched)
            pthread_join(simulation->philosophers[i].thread, NULL);
        if(simulation->philosophers[i].monitor_launched)
            pthread_join(simulation->philosophers[i].monitor_thread, NULL);
        i++;
    }
}
