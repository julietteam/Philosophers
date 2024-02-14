/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:27:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/14 17:43:24 by juandrie         ###   ########.fr       */
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
	int					is_dead;
	long long			last_meal_time;
	int					meals_eaten;
	int					sync;
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
	pthread_mutex_t	scheduler_mutex;
	pthread_mutex_t	death;
	pthread_mutex_t	write;
	int				stop;
	int				full_philosophers;
	long long		start_time;
}		t_simulation;


t_simulation	*init_simulation(t_simulation *simulation, int number_of_philosophers);
void			free_simulation(t_simulation *simulation);
int				start_philosopher_threads(t_simulation *simulation);
long long		current_timestamp_in_ms(void);
int				display_log(t_simulation *simulation, int philosopher_id, const char *action, t_philosopher *philosopher);
int				finalize_simulation(t_simulation *simulation);
int				join_philosopher_threads(t_simulation *simulation);
int				initialize_simulation(t_simulation *simulation, int ac, char **av);
void			free_simulation(t_simulation *simulation);
void			*philosopher_routine(void *arg);
int				take_forks(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork); 
int				eat(t_philosopher *philosopher, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
int				think_and_sleep(t_philosopher *philosopher);
int				update_scheduler(t_philosopher *philosopher);
int				check_philosopher_status(t_philosopher *philosopher);
int				monitor_philosopher_cycle(t_philosopher *philosopher);
void			set_sync(int i, t_simulation *simulation);
void			set_sync_2(int nb, int i, t_philosopher *philosopher);
void 			wait_after_thinking(long long delay);

#endif // PHILO_H