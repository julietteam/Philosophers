/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/10 22:46:04 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	int dead =  philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (philosopher->simulation->stop || philosopher->is_dead)
	{
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	pthread_mutex_lock(first_fork);
	if (stop || dead)
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (philosopher->simulation->stop)
	{
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	pthread_mutex_lock(second_fork);
	if (stop || dead)
	{
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	return (0);
	
}


int	eat(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;
	int			stop;
	
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = start_time;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (display_log(philosopher->simulation, philosopher->id, "is eating", philosopher) == -1)
	{
		return (-1);
	}	
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
	}
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	philosopher->meals_eaten++;
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (stop || philosopher->is_dead)
	{
		return (-1);
	}
	return(0);
}


int	think_and_sleep(t_philosopher *philosopher) 
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;
	int			stop;
	int			dead;

	printf("Philo %d dans think and sleep\n", philosopher->id);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (stop || dead)
	{
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "is sleeping", philosopher) == -1)
	{
		return (-1);
	}
	if (stop || dead)
	{
		return (-1);
	}
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	while (remaining > 0)
	{
		usleep(100);
		if(stop || dead)
		{
			return (-1);
		}
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	if (stop || dead)
	{
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", philosopher) == -1)
	{
		return (-1);
	}
	usleep(1000);
	return (0);
}


int	update_scheduler(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->params.number_of_times_each_philosopher_must_eat > 0 &&
	philosopher->meals_eaten >= philosopher->params.number_of_times_each_philosopher_must_eat)
	{
		philosopher->full = 1;
		philosopher->simulation->full_philosophers++;
		if (philosopher->simulation->full_philosophers == philosopher->params.number_of_philosophers)
		{
			pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
			return (-1);
		}
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0);
}






