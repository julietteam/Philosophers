/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:17:50 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/31 19:33:05 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	printf("Philosophe %d tente de prendre ses fourchettes.\n", philosopher->id);
	pthread_mutex_lock(first_fork);
	if (!philosopher->simulation->is_running)
	{
		printf("Philosophe %d arrêté en tentant de prendre la première fourchette.\n", philosopher->id);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	display_log(philosopher->simulation, philosopher->id, "has taken a fork");
	pthread_mutex_lock(second_fork);
	if (!philosopher->simulation->is_running)
	{
		printf("Philosophe %d arrêté en tentant de prendre la deuxième fourchette.\n", philosopher->id);
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	display_log(philosopher->simulation, philosopher->id, "has taken a fork");
}

void	put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	eat(t_philosopher *philosopher)
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	pthread_mutex_lock(&philosopher->mutex);
	pthread_mutex_lock(&philosopher->eating_mutex);
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_eat;
	philosopher->last_meal_time = current_timestamp_in_ms();
	display_log(philosopher->simulation, philosopher->id, "is eating");
	while (remaining > 0)
	{
		usleep(100);
		if (!philosopher->simulation->is_running)
		{
			pthread_mutex_unlock(&philosopher->mutex);
			pthread_mutex_unlock(&philosopher->eating_mutex);
			return ;
		}
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_eat - elapsed;
	}
	philosopher->meals_eaten++;
	pthread_mutex_unlock(&philosopher->eating_mutex);
	pthread_mutex_unlock(&philosopher->mutex);
}

void	think_and_sleep(t_philosopher *philosopher) 
{
	long long	start_time;
	long long	elapsed;
	long long	remaining;

	display_log(philosopher->simulation, philosopher->id, "is sleeping");
	start_time = current_timestamp_in_ms();
	remaining = philosopher->params.time_to_sleep;
	while (remaining > 0)
	{
		usleep(100);
		if (!philosopher->simulation->is_running)
		{
			printf("Philosophe %d arrêté pendant le sommeil.\n", philosopher->id);
			return ;
		}
		elapsed = current_timestamp_in_ms() - start_time;
		remaining = philosopher->params.time_to_sleep - elapsed;
	}
	display_log(philosopher->simulation, philosopher->id, "is thinking");
	usleep(1000);
}

void	update_scheduler(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);

	if (philosopher->params.number_of_times_each_philosopher_must_eat > 0 \
	&& philosopher->meals_eaten >= \
	philosopher->params.number_of_times_each_philosopher_must_eat)
	{
		philosopher->full = 1;
		philosopher->simulation->full_philosophers++;
		if (philosopher->simulation->full_philosophers == \
		philosopher->params.number_of_philosophers)
		{
			philosopher->simulation->is_running = 0;
		}
	}
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
}






