/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:27:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/07 13:48:16 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>


typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_params
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
}			t_params;

typedef struct s_philosopher
{
	bool				thread_launched;
	bool				monitor_launched;
	int					full;
	int					id;
	int					is_dead; // etat de vie du Philosophe : 1 si mort, 0 si vivant
	long long			last_meal_time;
	int					meals_eaten;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_t			thread;
	pthread_t			monitor_thread;
	t_params			params;
	struct s_simulation	*simulation;
	pthread_mutex_t		mutex;
}						t_philosopher;

typedef struct s_simulation
{
	t_params		*params;
	t_philosopher	*philosophers;
	t_fork			*forks;
	pthread_mutex_t	scheduler_mutex; // Mutex pour l'ordonnanceur
	pthread_mutex_t	death;
	pthread_mutex_t	write;
	int				current_philosopher_id;
	int				is_running;
	int				total_meals_eaten;
	int				full_philosophers;
	long long		start_time;
	int				count;
	int				flag;
}		t_simulation;


t_simulation	*init_simulation(t_simulation *simulation, int number_of_philosophers);
void			free_simulation(t_simulation *simulation);
void			*philosopher_routine(void *arg);
void			start_philosopher_threads(t_simulation *simulation);
long long		current_timestamp_in_ms(void);
// void display_log(int philosopher_id, const char *action);
void			display_log(t_simulation *simulation, int philosopher_id, const char *action);
void			finalize_simulation(t_simulation *simulation);
void			join_philosopher_threads(t_simulation *simulation);
void			start_simulation(t_simulation *simulation);
int				initialize_simulation(t_simulation *simulation, int ac, char **av);
void			free_simulation(t_simulation *simulation);
void			exit_cleanly(t_simulation *simulation, int philosopher_count);
void			*philosopher_routine(void *arg);
void			take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void			put_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void			eat(t_philosopher *philosopher);
void			think_and_sleep(t_philosopher *philosopher);
void			update_scheduler(t_philosopher *philosopher);

#endif // PHILO_H