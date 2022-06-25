/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:45:04 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/25 20:19:55 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	calc_time(t_bindle *bindle)
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

static int	my_sleep(int time_to_waste, t_bindle *bindle)
{
	struct timeval	now;
	struct timeval	start;

	if (time_to_waste == 0)
		return (0);
	gettimeofday(&start, NULL);
	gettimeofday(&now, NULL);
	while (time_diff(&now, &start) < time_to_waste)
	{
		usleep(50);
		(void) bindle;
		if (calc_time(bindle))
			return (1);
		gettimeofday(&now, NULL);
	}
	return (0);
}

void	loopy_philo(t_bindle *bindle)
{
	char	green_pass;

	green_pass = 0;
	while (bindle->meals < bindle->max_meals || bindle->max_meals == -1)
	{
		pthread_mutex_lock(bindle->fork_state_lock1);
		pthread_mutex_lock(bindle->fork_state_lock2);
		if (*(bindle->fork_state1) >= 0 && *(bindle->fork_state2) >= 0
			&& *(bindle->fork_state1) != bindle->id
			&& *(bindle->fork_state2) != bindle->id)
		{
			*(bindle->fork_state1) = -bindle->id;
			*(bindle->fork_state2) = -bindle->id;
			green_pass = 1;
		}
		pthread_mutex_unlock(bindle->fork_state_lock2);
		pthread_mutex_unlock(bindle->fork_state_lock1);
		//_______________________________
		calc_time(bindle);
		if (green_pass)
		{
			//if (bindle->time > 0)
			// if (bindle->time - bindle->countdown >=  bindle->die_time)
			// 	bindle->countdown = bindle->time + bindle->die_time;
			if (print_task2(bindle, "has taken a fork", GREEN)
				|| print_task2(bindle, "has taken a fork", GREEN)
				|| print_task2(bindle, "is_eating", GREEN))
			{
				pthread_mutex_lock(bindle->fork_state_lock1);
				pthread_mutex_lock(bindle->fork_state_lock2);
				*(bindle->fork_state1) = bindle->id;
				*(bindle->fork_state2) = bindle->id;
				pthread_mutex_unlock(bindle->fork_state_lock2);
				pthread_mutex_unlock(bindle->fork_state_lock1);
				break ;
			}
			my_sleep(bindle->eat_time, bindle);
			calc_time(bindle);
			bindle->countdown = bindle->time + bindle->die_time;
			//--------------
			pthread_mutex_lock(bindle->fork_state_lock1);
			pthread_mutex_lock(bindle->fork_state_lock2);
			*(bindle->fork_state1) = bindle->id;
			*(bindle->fork_state2) = bindle->id;
			green_pass = 0;
			pthread_mutex_unlock(bindle->fork_state_lock2);
			pthread_mutex_unlock(bindle->fork_state_lock1);
			//--------------
			calc_time(bindle);
			if (print_task2(bindle, "is sleeping", CYAN))
				break ;
			my_sleep(bindle->sleep_time, bindle);
			//----------------
			calc_time(bindle);
			if (print_task2(bindle, "is thinking", BLUE))
				break ;
			// if (bindle->eat_time * (bindle->type - 1) > bindle->sleep_time)
			// 	my_sleep(bindle->eat_time * (bindle->type - 1) - bindle->sleep_time - 1000, bindle);
			++(bindle->meals);
		}
		else
			usleep(100);
	}
}

void	*life_cycle2(void *bag)
{
	t_bindle	*bindle;

	bindle = bag;
	gettimeofday(&(bindle->end), NULL);
	usleep(200000 - time_diff(&(bindle->end), &(bindle->start)));
	gettimeofday(&(bindle->start), NULL);
	if (!my_sleep(bindle->eat_time * (bindle->id % bindle->type), bindle))
		loopy_philo(bindle);
	//ft_printf("i am %d\n", bindle->id);
	return (NULL);
}