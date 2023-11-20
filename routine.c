/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/17 15:48:48 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{ 
    if (!philosopher->simulation->is_running) 
        return;
    //printf("Philosophe %d prend les fourchettes\n", philosopher->id);    
    pthread_mutex_lock(first_fork);
    display_log(philosopher->simulation, philosopher->id, "has taken a fork");
    pthread_mutex_lock(second_fork);
    display_log(philosopher->simulation, philosopher->id, "has taken a fork");
}

void put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{
    //printf("Philosophe relâche les fourchettes\n");
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(second_fork);
}

/*
void eat(t_philosopher *philosopher) 
{

    pthread_mutex_lock(&philosopher->mutex); 
    pthread_mutex_lock(&philosopher->eating_mutex);
    //printf("%lld %d starts eating\n", current_timestamp_in_ms(), philosopher->id);
    philosopher->last_meal_time = current_timestamp_in_ms();
    display_log(philosopher->simulation, philosopher->id, "is eating");
    usleep(philosopher->params.time_to_eat * 1000);
    philosopher->meals_eaten++; 
    //printf("Philosopher %d has eaten %d times so far.\n", philosopher->id, philosopher->meals_eaten);
    pthread_mutex_unlock(&philosopher->eating_mutex);
    pthread_mutex_unlock(&philosopher->mutex); 

}
*/

// Exemple pour la fonction eat avec un délai d'attente plus court
void eat(t_philosopher *philosopher) {
    long long start_time, elapsed, remaining;

    pthread_mutex_lock(&philosopher->mutex); 
    pthread_mutex_lock(&philosopher->eating_mutex);

    philosopher->last_meal_time = current_timestamp_in_ms();
    display_log(philosopher->simulation, philosopher->id, "is eating");

    start_time = current_timestamp_in_ms();
    remaining = philosopher->params.time_to_eat;
    while (remaining > 0) {
        usleep(100);  // Délai d'attente plus court (100 microsecondes)
        elapsed = current_timestamp_in_ms() - start_time;
        remaining = philosopher->params.time_to_eat - elapsed;
    }

    philosopher->meals_eaten++;
    pthread_mutex_unlock(&philosopher->eating_mutex);
    pthread_mutex_unlock(&philosopher->mutex);
}



/*
void think_and_sleep(t_philosopher *philosopher)
{
    display_log(philosopher->simulation, philosopher->id, "is sleeping");
    usleep(philosopher->params.time_to_sleep * 1000);
    if (!philosopher->simulation->is_running) 
        return;
    display_log(philosopher->simulation, philosopher->id, "is thinking");
    usleep(1000);
}
*/

void think_and_sleep(t_philosopher *philosopher) 
{
    long long start_time, elapsed, remaining;

    // Partie pour dormir
    display_log(philosopher->simulation, philosopher->id, "is sleeping");
    
    start_time = current_timestamp_in_ms();
    remaining = philosopher->params.time_to_sleep;
    while (remaining > 0) {
        usleep(100);  // Délai d'attente plus court (100 microsecondes)
        elapsed = current_timestamp_in_ms() - start_time;
        remaining = philosopher->params.time_to_sleep - elapsed;
    }

    // Vérifiez si la simulation s'exécute toujours avant de penser
    if (!philosopher->simulation->is_running) {
        return;
    }

    // Partie pour penser
    display_log(philosopher->simulation, philosopher->id, "is thinking");
    usleep(1000); // Ce temps peut être ajusté en fonction des exigences
}


void update_scheduler(t_philosopher *philosopher) 
{
    pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        
    if (philosopher->params.number_of_times_each_philosopher_must_eat > 0 &&
        philosopher->meals_eaten >= philosopher->params.number_of_times_each_philosopher_must_eat)
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
    
    pthread_mutex_lock(&philosopher->mutex); 
    philosopher->is_dead = 0; 
    pthread_mutex_unlock(&philosopher->mutex); 
    philosopher->full = 0;
    philosopher->meals_eaten = 0;
    pthread_mutex_lock(&philosopher->mutex); 
    philosopher->last_meal_time = current_timestamp_in_ms();
    pthread_mutex_unlock(&philosopher->mutex); 
    pthread_mutex_t *first_fork, *second_fork;
    bool is_running_local;
    //printf("Philosophe %d démarre\n", philosopher->id);
    if (philosopher->simulation->params->number_of_philosophers == 1) 
    {
        pthread_mutex_lock(&philosopher->left_fork->mutex);
        display_log(philosopher->simulation, philosopher->id, "has taken a fork");
        usleep(philosopher->simulation->params->time_to_die * 1000);
        pthread_mutex_lock(&philosopher->mutex);
        philosopher->is_dead = 1;
        pthread_mutex_unlock(&philosopher->mutex);
        display_log(philosopher->simulation, philosopher->id, "died");
        pthread_mutex_unlock(&philosopher->left_fork->mutex);
        return NULL;
    }
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
    while (!philosopher->is_dead && (philosopher->simulation->is_running ||
            philosopher->params.number_of_times_each_philosopher_must_eat == 0))//(1) 
    {
        pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        is_running_local = philosopher->simulation->is_running;
        pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
        pthread_mutex_lock(&philosopher->mutex);
        if (philosopher->is_dead) // || !is_running_local
        {
            pthread_mutex_unlock(&philosopher->mutex);
            break;
        }
        pthread_mutex_unlock(&philosopher->mutex);
        if (!is_running_local && philosopher->is_dead) 
            break;
        take_forks(philosopher, first_fork, second_fork);
        if (philosopher->full != 1 && is_running_local)
        {
            eat(philosopher);
            update_scheduler(philosopher);
             if(!is_running_local)
                break;
        }
        put_forks(first_fork, second_fork); 
        pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        is_running_local = philosopher->simulation->is_running;
        pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
        if (philosopher->full == 1 || !is_running_local)
            break;
        think_and_sleep(philosopher);
    } 
    return (NULL);
}

void exit_cleanly(t_simulation *simulation, int philosopher_count)
{
    int i;

    i = 0;
    //printf("Nettoyage et sortie après l'erreur.\n"); 
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
    //printf("Sortie propre réalisée.\n");
    exit (1);
}
void *monitor_philosopher(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    bool running = philosopher->simulation->is_running;
    long long time_since_last_meal;
    while (running)
    {
        
        usleep(5000);
        pthread_mutex_lock(&philosopher->mutex); 
        time_since_last_meal = current_timestamp_in_ms() - philosopher->last_meal_time;
        pthread_mutex_unlock(&philosopher->mutex);  
        bool timed_out = time_since_last_meal > philosopher->params.time_to_die;
        if (timed_out && !philosopher->is_dead)
        {
            philosopher->is_dead = 1;
            display_log(philosopher->simulation, philosopher->id, "died");
            printf("Philosopher %d died. \n", philosopher->id);
            pthread_mutex_unlock(&philosopher->mutex);
            pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
            philosopher->simulation->is_running = 0;
            running = philosopher->simulation->is_running;
            pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
            if (!running)
                break;
        }
        else
        {
            pthread_mutex_unlock(&philosopher->mutex); 
            pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
            running = philosopher->simulation->is_running;
            pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
            if (running)
                break;
        }
    }
    return (NULL);
}


void start_philosopher_threads(t_simulation *simulation)
{
    int i;
    int j;
    int philosopher_count;
    bool error_occurred = false;
    
    i = 0;
    //printf("Démarrage des threads des philosophes.\n");
    philosopher_count = simulation->params->number_of_philosophers;
    while (i < philosopher_count)
    {
        if (pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
        {
            simulation->philosophers[i].thread_launched = false;
            //printf("Thread pour philosophe %d lancé.\n", i);
            error_occurred = true;
        }
        else
            simulation->philosophers[i].thread_launched = true;
        if (pthread_create(&simulation->philosophers[i].monitor_thread, NULL, monitor_philosopher, &simulation->philosophers[i]) != 0)
        {   
            simulation->philosophers[i].monitor_launched = false;
            error_occurred = true;
        }
        else
            simulation->philosophers[i].thread_launched = true;
        i++;
    }
    if (error_occurred)
    {
        j = 0;
        while (j < i)
        {
            if (simulation->philosophers[j].thread_launched)
                pthread_join(simulation->philosophers[j].thread, NULL);
            if (simulation->philosophers[j].monitor_launched)
                pthread_join(simulation->philosophers[j].monitor_thread, NULL);
            j++;
        }
        exit_cleanly(simulation, philosopher_count);
    }
}

