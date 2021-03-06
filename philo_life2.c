/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:45:04 by ayassin           #+#    #+#             */
/*   Updated: 2022/07/28 18:28:45 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Assumptions
1)	philo has to finish eating to extend his life span by
time_to_die

2) microseconds are not a rounding error, they are tracked
even if they are not printed
*/

int	get_fork(t_bindle *bindle)
{
	int	green_pass;

	green_pass = 0;
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
	return (green_pass);
}

int	leave_fork(t_bindle *bindle, int fork_state)
{
	pthread_mutex_lock(bindle->fork_state_lock1);
	pthread_mutex_lock(bindle->fork_state_lock2);
	*(bindle->fork_state1) = fork_state;
	*(bindle->fork_state2) = fork_state;
	pthread_mutex_unlock(bindle->fork_state_lock2);
	pthread_mutex_unlock(bindle->fork_state_lock1);
	return (1);
}

int	eat(t_bindle *bindle)
{
	if (print_task2(bindle, "has taken a fork", GREEN)
		|| print_task2(bindle, "has taken a fork", GREEN)
		|| print_task2(bindle, "is_eating", GREEN))
		return (leave_fork(bindle, 0));
	my_sleep(bindle->eat_time, bindle);
	calc_time(bindle);
	bindle->countdown = bindle->time + bindle->die_time;
	return (0);
}

void	loopy_philo(t_bindle *bindle)
{
	char	green_pass;

	while (bindle->meals < bindle->max_meals || bindle->max_meals == -1)
	{
		green_pass = get_fork(bindle);
		if (calc_time(bindle) && !green_pass)
			return ;
		if (green_pass)
		{
			if (eat(bindle))
				return ;
			leave_fork(bindle, bindle->id);
			calc_time(bindle);
			if (print_task2(bindle, "is sleeping", CYAN))
				break ;
			my_sleep(bindle->sleep_time, bindle);
			calc_time(bindle);
			if (print_task2(bindle, "is thinking", BLUE))
				break ;
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
	if (bindle->fork_state_lock1 == bindle->fork_state_lock2)
	{
		my_sleep(bindle->die_time + 20, bindle);
		return (NULL);
	}
	if (!my_sleep(bindle->eat_time * (bindle->id % bindle->type), bindle))
		loopy_philo(bindle);
	return (NULL);
}