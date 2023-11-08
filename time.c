/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/08 19:04:57 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Fonction pour obtenir le timestamp actuel en millisecondes
long long current_timestamp_in_ms() 
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return (milliseconds);
}

// Fonction pour afficher les logs
void display_log(t_simulation *simulation, int philosopher_id, const char *action) 
{
        pthread_mutex_lock(&log_mutex);
        // printf("%lld %d %s\n", current_timestamp_in_ms(), philosopher_id, action);
        printf("%lld %d %s\n", current_timestamp_in_ms() - simulation->start_time, philosopher_id, action);
        pthread_mutex_unlock(&log_mutex);
}