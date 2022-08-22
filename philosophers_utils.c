/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 11:47:53 by ayassin           #+#    #+#             */
/*   Updated: 2022/08/22 18:18:21 by ayassin          ###   ########.fr       */
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

long	philo_atoi(char *str, int factor)
{
	unsigned long	num;

	num = 0;
	if (*str == '+')
		str++;
	if (*str < '0' || *str > '9')
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0');
		if (num >> 31)
			return (-1);
		str++;
	}
	if (*str)
		return (-1);
	return (num * factor);
}

int	print_task2(t_bindle *bag, char *task, char *color)
{
	char	death;
	long	time;

	death = 0;
	gettimeofday(&(bag->end), NULL);
	if (task != NULL)
		bag->time = time_diff(&(bag->end), &(bag->start));
	time = bag->time / 1000;
	pthread_mutex_lock(bag->common_lock);
	if (*(bag->death) != 0)
		death = 1;
	else if ((int) bag->time < 0)
	{
		*(bag->death) = 1;
		printf("%s%ld %d %s\n%s", RED, -time, bag->id, "died", RST_COLOR);
		death = 1;
	}
	else if (task)
		printf("%s%ld %d %s\n%s", color, time, bag->id, task, RST_COLOR);
	else
		printf("%s%ld %d has taken a fork\n%ld %d has taken a fork\n%ld %d %s\n%s",
			color, time, bag->id, time, bag->id, time, bag->id,
			"is eating", RST_COLOR);
	pthread_mutex_unlock(bag->common_lock);
	return (death);
}

int	calc_time(t_bindle *bindle)
{
	gettimeofday(&(bindle->end), NULL);
	bindle->time = time_diff(&(bindle->end), &(bindle->start));
	if (bindle->time < 0)
		return (print_task2(bindle, NULL, NULL));
	else if (bindle->time > bindle->countdown)
	{
		bindle->time *= -1;
		return (print_task2(bindle, NULL, NULL));
	}
	return (0);
}

int	my_sleep2(int time_to_waste, t_bindle *bindle, t_timeval start)
{
	struct timeval	now;

	if (time_to_waste == 0)
		return (0);
	gettimeofday(&now, NULL);
	while (time_diff(&now, &start) < time_to_waste)
	{
		usleep(50);
		if (calc_time(bindle))
			return (1);
		gettimeofday(&now, NULL);
	}
	return (0);
}
