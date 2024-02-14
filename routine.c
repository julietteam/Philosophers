/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/14 19:40:08 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	int stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	int dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (stop || dead)
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{	
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (stop || dead)
	{
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (stop || dead)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "has taken a fork", philosopher) == -1)
	{	
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (-1);
	}
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	pthread_mutex_lock(&philosopher->simulation->death);
	dead = philosopher->is_dead;
	pthread_mutex_unlock(&philosopher->simulation->death);
	if (stop || dead)
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
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
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (stop)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	if (display_log(philosopher->simulation, philosopher->id, "is eating", philosopher) == -1)
		return (-1);
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
	}
	philosopher->meals_eaten++;
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (stop)
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return (-1);
	}
	return(0);
}

void wait_after_thinking(long long delay)
{
    long long time_now = current_timestamp_in_ms();

    while (current_timestamp_in_ms() - time_now < delay)
	{
        usleep(50);
    }
}


void	set_sync(int i, t_simulation *simulation)
{
	int nb = simulation->params->number_of_philosophers;
	
	if (nb == 3)
	{
		if (i % 2 == 0)
			simulation->philosophers[i].sync = 50;
		else
			simulation->philosophers[i].sync = 100;
	}
	set_sync_2(nb, i, simulation->philosophers);
}

void	set_sync_2(int nb, int i, t_philosopher *philosopher)
{
	if (nb % 2 != 0)
	{
		if ((philosopher[i].simulation->params->time_to_eat * 3 <= philosopher[i].simulation->params->time_to_die) && philosopher[i].simulation->params->time_to_sleep < philosopher[i].simulation->params->time_to_eat)
		{
			(philosopher[i].sync += philosopher[i].simulation->params->time_to_eat - philosopher[i].simulation->params->time_to_sleep);
		}
		else if (philosopher[i].simulation->params->time_to_eat * 3 >  philosopher[i].simulation->params->time_to_die && philosopher[i].simulation->params->time_to_sleep + philosopher[i].simulation->params->time_to_eat < philosopher[i].simulation->params->time_to_die)
		{
			(philosopher[i].sync += philosopher[i].simulation->params->time_to_die - philosopher[i].simulation->params->time_to_eat + philosopher[i].simulation->params->time_to_sleep);
		}
	}
}

int	think_and_sleep(t_philosopher *philosopher) 
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;
	int			stop;


	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (stop)
		return (-1);
	if (display_log(philosopher->simulation, philosopher->id, "is sleeping", philosopher) == -1)
		return (-1);
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	while (remaining > 0)
	{
		usleep(100);
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	stop = philosopher->simulation->stop;
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	if (stop)
		return (-1);
	if (display_log(philosopher->simulation, philosopher->id, "is thinking", philosopher) == -1)
		return (-1);
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






