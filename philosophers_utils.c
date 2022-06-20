/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 11:47:53 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/20 18:33:26 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	time_diff(struct timeval *end, struct timeval *start)
{
	int	time;

	time = (end->tv_sec - start->tv_sec) * 1000;
	time += (end->tv_usec - start->tv_usec) / 1000;
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

int	print_task(t_bag *bag, char *task, char *color)
{
	//check if somone has died
	if (*(bag->death) == 1)
		return (1);
	if (bag->time < -1)
	{
		*(bag->death) = 1;
		printf("%s%d %d %s\n%s", BLACK, -bag->time, bag->id, "is dead", RESET_COLOR);
		return (1);
	}
	ft_printf("%s%d %d %s\n%s", color, bag->time, bag->id, task, RESET_COLOR);
	return (0);
}
