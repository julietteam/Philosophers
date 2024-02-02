/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/31 14:48:11 by juandrie         ###   ########.fr       */
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
		return (EXIT_FAILURE);

	start_simulation(&simulation);
	join_philosopher_threads(&simulation);
	finalize_simulation(&simulation);

	return (0);
}
