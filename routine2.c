/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:30:40 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/08 18:30:05 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_philosopher_routine(t_philosopher *philosopher, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork)
{
	philosopher->is_dead = 0;
	philosopher->full = 0;
	philosopher->meals_eaten = 0;
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	philosopher->last_meal_time = current_timestamp_in_ms();
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
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
	return (0);
}
int handle_single_philosopher(t_philosopher *philosopher)
{
	if (philosopher->simulation->params->number_of_philosophers == 1)
	{
		if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
		{
			return (-1);
		}	
		usleep(philosopher->simulation->params->time_to_die * 1000);
		if (display_log(philosopher->simulation, philosopher->id, "died", philosopher) == -1)
		{
			return (-1);
		}	
		return (-1);
	}
	return (0);
}

int	philosopher_actions(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	
	if(!philosopher->is_dead)
	{
		if (take_forks(philosopher, first_fork, second_fork) == -1)
			return (-1);
		if (philosopher->full != 1)
		{
			if (eat(philosopher) == -1)
				return (-1);
			if (update_scheduler(philosopher) == -1)
				return (-1);
		}
		pthread_mutex_unlock(first_fork);
	}
	return (0);
}

int	philosopher_life_cycle(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{

	if (display_log(philosopher->simulation, philosopher->id, "is thinking", philosopher) == -1)
	{
		return (-1);
	}	
	while (!philosopher->is_dead)//(1)
	{
		if (philosopher_actions(philosopher, first_fork, second_fork) == -1)
			return (-1);
		if (philosopher->full == 1)
			return (-1);
		if (think_and_sleep(philosopher) == -1)
			return (-1);
	}
	return (0);
}


void	*philosopher_routine(void *arg)
{
	t_philosopher		*philosopher;
	pthread_mutex_t		*first_fork;
	pthread_mutex_t		*second_fork;

	philosopher = (t_philosopher *)arg;
	philosopher->exit_status = 0;
	if (handle_single_philosopher(philosopher) == -1)
		philosopher->exit_status = -1;
	initialize_philosopher_routine(philosopher, &first_fork, &second_fork);
	if (philosopher_life_cycle(philosopher, first_fork, second_fork) == -1)
		philosopher->exit_status = -1;
	return (NULL);
}
