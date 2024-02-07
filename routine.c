/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/07 19:38:28 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	bool is_running = philosopher->simulation->is_running;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (!is_running || philosopher->is_dead)
	{
		return ;
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	
	// int left_id = philosopher->id;
    // int right_id = philosopher->id;

    // if (left_id < right_id)
	// {
	// 	*first_fork = philosopher->left_fork->mutex;
    //     *second_fork = philosopher->right_fork->mutex;
    // } 
	// else
	// {
	// 	*first_fork = philosopher->right_fork->mutex;
    //     *second_fork = philosopher->left_fork->mutex;
    // }
	pthread_mutex_lock(first_fork);
	if (!philosopher->simulation->is_running || philosopher->is_dead) // || philosopher->is_dead
	{
		return ;
	}
	pthread_mutex_lock(&philosopher->simulation->write);
	display_log(philosopher->simulation, philosopher->id, "has taken a fork");
	pthread_mutex_unlock(&philosopher->simulation->write);
	pthread_mutex_lock(second_fork);
	if (!philosopher->simulation->is_running || philosopher->is_dead) //!philosopher->simulation->is_running || 
	{
		return ;
	}
	pthread_mutex_lock(&philosopher->simulation->write);
	display_log(philosopher->simulation, philosopher->id, "has taken a fork");
	pthread_mutex_unlock(&philosopher->simulation->write);
	pthread_mutex_unlock(second_fork);
	return ;
	
}


// void	put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
// {
// 	pthread_mutex_unlock(first_fork);
// 	pthread_mutex_unlock(second_fork);
// }

void	eat(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;
	bool		is_running;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->is_dead || !philosopher->simulation->is_running)
	{
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return;
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = start_time; //current_timestamp_in_ms();
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->write);
	display_log(philosopher->simulation, philosopher->id, "is eating");
	pthread_mutex_unlock(&philosopher->simulation->write);
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
        is_running = philosopher->simulation->is_running;
       	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		if (!is_running)
		{
			return ;
		}
	}
	philosopher->meals_eaten++;
}


void	think_and_sleep(t_philosopher *philosopher) 
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;
	bool		is_running;

	pthread_mutex_lock(&philosopher->simulation->write);
	display_log(philosopher->simulation, philosopher->id, "is sleeping");
	pthread_mutex_unlock(&philosopher->simulation->write);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	while (remaining > 0)
	{
		usleep(100);
		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
    	is_running = philosopher->simulation->is_running;
    	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		if (!is_running) //|| philosopher->is_dead//(!philosopher->simulation->is_running)
		{
			return ;
		}
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	if (!philosopher->simulation->is_running) //|| philosopher->is_dead
	{
		return;
    }
	pthread_mutex_lock(&philosopher->simulation->write);
	display_log(philosopher->simulation, philosopher->id, "is thinking");
	pthread_mutex_unlock(&philosopher->simulation->write);
	usleep(1000);
}


void	update_scheduler(t_philosopher *philosopher)
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






