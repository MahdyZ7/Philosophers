/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 11:47:53 by ayassin           #+#    #+#             */
/*   Updated: 2022/08/23 16:02:10 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief calculate the time diffrence between end and start in microseconds
 * 
 * @param end time
 * @param start time
 * @return long long diffrence
 */
long long	time_diff(struct timeval *end, struct timeval *start)
{
	long long	time;

	time = ((long long) (end->tv_sec - start->tv_sec)) * 1000 * 1000;
	time += (end->tv_usec - start->tv_usec);
	return (time);
}

/**
 * @brief atoi for philos project. turn string to ling
 * 
 * @param str number string that has to be an int
 * @param factor magnitude multiplyer for number
 * @return the number atoied or -1 if input is invalid
 */
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

/**
 * @brief log when an event in a philo's life happens. mutex locked to prevent mixup.
 * if time is negative the philo has died. if the death variable 
 * 
 * @param bag info about the philo
 * @param task what the philo will do (NULL means die)
 * @param color color of print statment
 * @return 1 for death , 0 otherwise
 */
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

/**
 * @brief cacluate the time passed since the start or a project
 * 
 * @param bindle philo info
 * @return 1 if philo is dead, 0 otherwise
 */
int	calc_time(t_bindle *bindle)
{
	gettimeofday(&(bindle->end), NULL);
	bindle->time = time_diff(&(bindle->end), &(bindle->start));
	if (bindle->time > bindle->countdown)
	{
		bindle->time *= -1;
		return (print_task2(bindle, NULL, NULL));
	}
	return (0);
}

/**
 * @brief sleep function for philos
 * 
 * @param time_to_waste how much time to waste
 * @param bindle philo info
 * @param start sudo start time for time wasting
 * @return 1 if a philo dies, 0 otherwise
 */
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
