/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 11:47:53 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/19 14:17:09 by ayassin          ###   ########.fr       */
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

int	print(int time, int id, char *task, char *color)
{
	//check if somone has died
	ft_printf("%s%d %d %s\n%s", color, time, id, task, RESET_COLOR);
	return (0);
}
