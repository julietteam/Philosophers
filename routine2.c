/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:30:40 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/12 19:01:21 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_philosopher_routine(t_philosopher *philosopher, pthread_mutex_t **first_fork, pthread_mutex_t **second_fork)
{
	pthread_mutex_lock(&philosopher->simulation->death);
	philosopher->is_dead = 0;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	philosopher->full = 0;
	philosopher->meals_eaten = 0;
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
			return (-1);
		usleep(philosopher->simulation->params->time_to_die * 1000);
		return (-1);
	}
	return (0);
}

int	philosopher_actions(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	int	stop;
	int	dead;
	
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (!stop || !dead)
	{
		if (take_forks(philosopher, first_fork, second_fork) == -1)
			return (-1);
		if (philosopher->full != 1)
		{ 
			if (eat(philosopher, first_fork, second_fork) == -1)
			{
				return (-1);
			}
			pthread_mutex_unlock(first_fork);
			pthread_mutex_unlock(second_fork);
			if (update_scheduler(philosopher) == -1)
				return (-1);
		}
	}
	return (0);
}

int	philosopher_life_cycle(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) 
{

	
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", philosopher) == -1)
			return (-1);
	pthread_mutex_lock(&philosopher->simulation->death);
	bool dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (!dead || !stop)
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
	pthread_mutex_lock(&philosopher->simulation->death);
	int dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (!dead && !stop)
    {
		if (handle_single_philosopher(philosopher) == -1)
			break;
		initialize_philosopher_routine(philosopher, &first_fork, &second_fork);
		if (philosopher_life_cycle(philosopher, first_fork, second_fork) == -1)
			break ;
	}
	return (NULL);
}
