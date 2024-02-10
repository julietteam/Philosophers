/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/09 22:17:29 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	check_arguments(int ac, char **av)
{
	if (ac < 5)
	{
		printf("Usage: %s number_of_philosophers\
		time_to_die time_to_eat time_to_sleep \
		[number_of_times_each_philosopher_must_eat]\n", av[0]);
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_simulation	simulation;

	if (!check_arguments(ac, av))
		return (1);

	if (!initialize_simulation(&simulation, ac, av))
		return (-1);
	if (start_philosopher_threads(&simulation) == -1)
		return (-1);
	if (join_philosopher_threads(&simulation) == -1)
		return (-1);
	if (finalize_simulation(&simulation) == -1)
		return (-1);
	return (0);
}
