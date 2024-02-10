/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/10 16:00:05 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Fonction pour obtenir le timestamp actuel en millisecondes
long long	current_timestamp_in_ms(void)
{
	struct timeval	te;
	long long		milliseconds;

	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return (milliseconds);
}

// Fonction pour afficher les logs
int	display_log(t_simulation *simulation, int philosopher_id, const char *action, t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->simulation->death);
	if (philosopher->is_dead == 1)
	{
		pthread_mutex_unlock(&philosopher->simulation->death);
	}
	pthread_mutex_lock(&philosopher->simulation->write);
	printf("%lld %d %s\n", current_timestamp_in_ms() - simulation->start_time, philosopher_id, action);
	pthread_mutex_unlock(&philosopher->simulation->write);
	pthread_mutex_unlock(&philosopher->simulation->death);
	return (0);
}
