/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:45:04 by ayassin           #+#    #+#             */
/*   Updated: 2022/08/21 13:53:53 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* Assumptions
1)	A young philo has to start eating to extend his life span by
time_to_die

2) microseconds are not a rounding error, they are tracked
even if they are not printed

3) the fork state is tracked. The fork itself is fiction but just becuase it
lives in our heads does not mean it is not real
*/

/**
 * @brief Check the fork state and update it if it is available.
 * fork_state should not be negive to be avaliable. the fork state should not be
 * the same as the philo id (to prevent greedy philos)
 * 
 * @param bindle 
 * @return int returns 1 if philo grabed fork, 0 otherwise
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

/**
 * @brief Leave fork by changing fork state
 * 
 * @param bindle 
 * @param fork_state the updated fork state will be (philo->id normally) 
 * @return returns 1, stop philo thread at death
 */
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

// atio and sleep times
// modifiy atoi  by adding an argument
/**
 * @brief eating, print
 * 
 * @param bindle 
 * @return int 
 */
int	eat(t_bindle *bindle)
{
	t_timeval	now;

	calc_time(bindle);
	gettimeofday(&now, NULL);
	bindle->countdown = bindle->time + bindle->die_time;
	if (print_task2(bindle, NULL, GREEN))
		return (leave_fork(bindle, 0));
	my_sleep2(bindle->eat_time, bindle, now);
	calc_time(bindle);
	return (0);
}

// int	eat(t_bindle *bindle)
// {
// 	calc_time(bindle);
// 	if (print_task2(bindle, NULL, GREEN))
// 		return (leave_fork(bindle, 0));
// 	my_sleep(bindle->eat_time, bindle);
// 	calc_time(bindle);
// 	bindle->countdown = bindle->time + bindle->die_time;
// 	return (0);
// }

void	loopy_philo(t_bindle *bindle)
{
	char		green_pass;
	t_timeval	now;

	while (bindle->meals < bindle->max_meals || bindle->max_meals == -1)
	{
		green_pass = get_fork(bindle);
		if (green_pass)
		{
			if (eat(bindle))
				return ;
			leave_fork(bindle, bindle->id);
			calc_time(bindle);
			gettimeofday(&now, NULL);
			if (print_task2(bindle, "is sleeping", CYAN))
				break ;
			my_sleep2(bindle->sleep_time, bindle, now);
			calc_time(bindle);
			if (print_task2(bindle, "is thinking", BLUE))
				break ;
			++(bindle->meals);
		}
		else if (calc_time(bindle))
			return ;
		else
			usleep(100);
	}
}

void	*life_cycle2(void *bag)
{
	t_bindle	*bindle;
	long		syncsleep;

	bindle = bag;
	gettimeofday(&(bindle->end), NULL);
	if (bindle->max_meals == 0)
		return (NULL);
	syncsleep = bindle->die_time - time_diff(&(bindle->end), &(bindle->start));
	if (syncsleep < 0)
	{
		calc_time(bindle);
		return (NULL);
	}
	syncsleep = bindle->eat_time * (1 + bindle->id % bindle->type)
		- time_diff(&(bindle->end), &(bindle->start));
	if (bindle->fork_state_lock1 == bindle->fork_state_lock2 || syncsleep < 0)
	{
		my_sleep(bindle->die_time + 20, bindle);
		return (NULL);
	}
	syncsleep = bindle->eat_time * (bindle->id % bindle->type)
		- time_diff(&(bindle->end), &(bindle->start));
	if (!my_sleep(syncsleep, bindle))
		loopy_philo(bindle);
	return (NULL);
}
