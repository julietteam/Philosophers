/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:30:40 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/02 16:56:11 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_philosopher_routine(t_philosopher *philosopher, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork)
{
	philosopher->is_dead = 0;
	philosopher->full = 0;
	philosopher->meals_eaten = 0;
	philosopher->last_meal_time = current_timestamp_in_ms();
	if (philosopher->id % 2 == 0)
	{
		*first_fork = &philosopher->left_fork->mutex;
		*second_fork = &philosopher->right_fork->mutex;
	}
	else
	{
		*first_fork = &philosopher->right_fork->mutex;
		*second_fork = &philosopher->left_fork->mutex;
	}
}
int handle_single_philosopher(t_philosopher *philosopher)
{
	if (philosopher->simulation->params->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philosopher->left_fork->mutex);
		display_log(philosopher->simulation, philosopher->id, "has taken a fork");
		usleep(philosopher->simulation->params->time_to_die * 1000);
		pthread_mutex_unlock(&philosopher->left_fork->mutex);
		return (1);
	}
	return (0);
}

void	philosopher_actions(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork, bool *is_running_local)
{
	bool	is_running;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	is_running = *is_running_local;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (!is_running || philosopher->is_dead)
		return;
	take_forks(philosopher, first_fork, second_fork);
	if (philosopher->full != 1 && is_running) //&& is_running
	{
		eat(philosopher);
		update_scheduler(philosopher);
	}
	put_forks(first_fork, second_fork);
}

void philosopher_life_cycle(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{
	bool	is_running_local;

	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	display_log(philosopher->simulation, philosopher->id, "is thinking");
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (1)
	{

		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		is_running_local = philosopher->simulation->is_running;
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);

		if (!is_running_local)
			break;
		philosopher_actions(philosopher, first_fork, second_fork, &is_running_local);

		pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
		is_running_local = philosopher->simulation->is_running;
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		
		if (!is_running_local || philosopher->full == 1)
			break;
		think_and_sleep(philosopher);
	}
}


void	*philosopher_routine(void *arg)
{
	t_philosopher		*philosopher;
	pthread_mutex_t		*first_fork;
	pthread_mutex_t		*second_fork;

	philosopher = (t_philosopher *)arg;
	if (handle_single_philosopher(philosopher))
		return (NULL);
	initialize_philosopher_routine(philosopher, &first_fork, &second_fork);
	philosopher_life_cycle(philosopher, first_fork, second_fork);
	return (NULL);
}
