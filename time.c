/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/20 18:05:59 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	current_timestamp_in_ms(void)
{
	struct timeval	te;
	long long		milliseconds;

	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return (milliseconds);
}

int	display_log(t_simulation *simulation, int philosopher_id, \
const char *action, t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->scheduler_mutex);
	if (philosopher->simulation->stop == 1)
	{
		pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
		return (-1);
	}
	pthread_mutex_lock(&philosopher->simulation->write);
	printf("%lld %d %s\n", current_timestamp_in_ms() - \
	simulation->start_time, philosopher_id, action);
	pthread_mutex_unlock(&philosopher->simulation->write);
	pthread_mutex_unlock(&philosopher->simulation->scheduler_mutex);
	return (0);
}
