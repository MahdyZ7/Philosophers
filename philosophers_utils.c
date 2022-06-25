/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 11:47:53 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/25 20:31:06 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	time_diff(struct timeval *end, struct timeval *start)
{
	long	time;

	time = (end->tv_sec - start->tv_sec) * 1000 * 1000;
	time += (end->tv_usec - start->tv_usec);
	return (time);
}

int	minitalk_atoi(char *str, int *valid_flag)
{
	unsigned long	num;

	num = 0;
	if (*str == '+')
		str++;
	if (*str < '0' || *str > '9')
		*valid_flag = 0;
	while (*str >= '0' && *str <= '9' && *valid_flag)
	{
		num = num * 10 + *str - '0';
		if (num >> 31)
			*valid_flag = 0;
		str++;
	}
	if (*str)
		*valid_flag = 0;
	return (num);
}

int	print_task2(t_bindle *bag, char *task, char *color)
{
	char	death;

	death = 0;
	pthread_mutex_lock(bag->common_lock);
	if (*(bag->death) == 1)
		death = 1;
	else if ((int) bag->time < 0)
	{
		*(bag->death) = 1;
		ft_printf("%s%d %d %s\n%s", RED, -bag->time / 1000, bag->id, "is dead", RESET_COLOR);
		death = 1;
	}
	else
		printf("%s%ld %d %s\n%s", color, bag->time / 1000, bag->id, task, RESET_COLOR);
	pthread_mutex_unlock(bag->common_lock);
	return (death);
}

// int	add_task(t_bindle *bag, char *task, char *color)
// {
// 	char	death;

// 	death = 0;
// 	pthread_mutex_lock(bag->common_lock);
// 	if (*(bag->death) == 1)
// 		death = 1;
// 	else if ((int) bag->time < 0)
// 	{
// 		*(bag->death) = 1;
// 		ft_lstadd_back()
// 		ft_printf("%s%d %d %s\n%s", RED, -bag->time / 1000, bag->id, "is dead", RESET_COLOR);
// 		death = 1;
// 	}
// 	else
// 		printf("%s%ld %d %s\n%s", color, bag->time / 1000, bag->id, task, RESET_COLOR);
// 	pthread_mutex_unlock(bag->common_lock);
// 	return (death);
// }
