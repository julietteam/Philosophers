/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/02 18:15:19 by juandrie         ###   ########.fr       */
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

void take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{
    pthread_mutex_lock(first_fork);
    display_log(philosopher->id, "has taken a fork");
    pthread_mutex_lock(second_fork);
    display_log(philosopher->id, "has taken a fork");
}

void put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}

void eat(t_philosopher *philosopher) 
{
    
    philosopher->last_meal_time = current_timestamp_in_ms();
    display_log(philosopher->id, "is eating");
    usleep(philosopher->params.time_to_eat);
}

void think_and_sleep(t_philosopher *philosopher) 
{
    display_log(philosopher->id, "is thinking");
    usleep(10000);
    display_log(philosopher->id, "is sleeping");
    usleep(philosopher->params.time_to_sleep);  // Simule le temps de sommeil
}

void wait_for_turn(t_philosopher *philosopher) 
{
    pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
    while (philosopher->id != philosopher->simulation->current_philosopher_id) 
    {
        pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
        // usleep(5000);  // Attente active
        pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
    }
}

void update_scheduler(t_philosopher *philosopher) 
{
    philosopher->simulation->current_philosopher_id = (philosopher->id % philosopher->params.number_of_philosophers) + 1;
    pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
}

void *philosopher_routine(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    pthread_barrier_init(&simulation->start_barrier, NULL, );
    philosopher->last_meal_time = current_timestamp_in_ms(); 
    philosopher->is_dead = 0;

    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;
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

    while (!philosopher->is_dead) 
    {
        pthread_barrier_wait(&philosopher->simulation->start_barrier);
        think_and_sleep(philosopher);
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead) 
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);

        wait_for_turn(philosopher);
        
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead) 
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);

        take_forks(philosopher, first_fork, second_fork);
        
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead) 
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);

        eat(philosopher);
        
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead) 
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);

        update_scheduler(philosopher);
        put_forks(first_fork, second_fork);
    }

    return NULL;
}


void *monitor_philosopher(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *) arg;
    
    while (true) 
    {
        usleep(1000);
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead)
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        if (current_timestamp_in_ms() - philosopher->last_meal_time > philosopher->params.time_to_die) 
        {
            display_log(philosopher->id, "died");
            philosopher->is_dead = 1;
            pthread_mutex_unlock(&philosopher->mutex);
            pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex); // Release the lock before exiting
            // pthread_exit(NULL); 
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);
    }
    return(NULL);
}

void start_philosopher_threads(t_simulation *simulation)
{
    int i;

    i = 0;
    while (i < simulation->params.number_of_philosophers)
    {
        if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
        {
            printf("Error creating thread\n");
            exit(1);
        }
        if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, monitor_philosopher, &simulation->philosophers[i]) != 0)
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
        pthread_join(simulation->philosophers[i].monitor_thread, NULL);
        i++;
    }
}

