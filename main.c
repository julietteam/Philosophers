/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:26:11 by juandrie          #+#    #+#             */
/*   Updated: 2024/02/15 14:27:49 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_numeric_positive(char *str)
{
	if (*str == '-' || (*str == '0' && str[1] != '\0'))
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	compare_with_max(char *str)
{
	char	*max_str;
	int		len;
	int		i;

	max_str = "2147483647";
	len = ft_strlen(str);
	i = 0;
	if (len > ft_strlen(max_str))
		return (0);
	if (len == ft_strlen(max_str))
	{
		while (i < len)
		{
			if (str[i] > max_str[i])
				return (0);
			else if (str[i] < max_str[i])
				break ;
			i++;
		}
	}
	return (1);
}

int	is_valid(char *str)
{
	if (!is_numeric_positive(str))
		return (0);
	return (compare_with_max(str));
}

int	check_arguments(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
	{
		printf("Usage: %s number_of_philosophers\
		time_to_die time_to_eat time_to_sleep \
		[number_of_times_each_philosopher_must_eat]\n", av[0]);
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		if (!is_valid(av[i]))
		{
			printf("Error: All arguments must be positive integers.\n");
			return (0);
		}
		i++;
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
