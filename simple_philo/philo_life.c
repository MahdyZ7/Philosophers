/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:45:04 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/23 09:36:15 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	calc_time(t_bag *bag)
{
	gettimeofday(&(bag->end), NULL);
	bag->time = time_diff(&(bag->end), &(bag->start));
	if (bag->time * 1000 > bag->death_clock)
		bag->time *= -1;
	return (0);
}

static int	my_sleep(int time_to_waste, t_bag *bag)
{
	struct timeval	now;
	struct timeval	start;

	gettimeofday(&start, NULL);
	gettimeofday(&now, NULL);
	while (time_diff(&now, &start) * 1000 < time_to_waste + 200)
	{
		usleep(50);
		gettimeofday(&now, NULL);
		calc_time(bag);
		if (bag->time < 0)
		{
			pthread_mutex_lock(bag->common_lock);
			print_task(bag, NULL, NULL);
			pthread_mutex_unlock(bag->common_lock);
		}
	}
	return (0);
}

static void	loopy_philo(t_bag *my_bag)
{
	struct timeval	now;

	while (my_bag->no_of_meals < my_bag->max_no_of_meals
		|| my_bag->max_no_of_meals == -1)
	{
		calc_time(my_bag);
		if (my_bag->time != -1)
		{
			gettimeofday(&now, NULL);
			my_bag->death_clock = my_bag->time * 1000 + my_bag->time_to_die;
		} //position is uncertian
		pthread_mutex_lock(my_bag->common_lock);
		// print_task(my_bag, "has taken a fork", GREEN);
		// print_task(my_bag, "has taken a fork", GREEN);
		if (print_task(my_bag, "is_eating", GREEN))
		{
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		pthread_mutex_unlock(my_bag->common_lock);
		my_sleep(my_bag->time_to_eat, my_bag);
		//--------------
		calc_time(my_bag);
		pthread_mutex_lock(my_bag->common_lock);
		if (print_task(my_bag, "is sleeping", CYAN))
		{
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		pthread_mutex_unlock(my_bag->common_lock);
		my_sleep(my_bag->time_to_sleep, my_bag);
		//----------------
		calc_time(my_bag);
		pthread_mutex_lock(my_bag->common_lock);
		if (print_task(my_bag, "is thinking", BLUE))
		{
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		pthread_mutex_unlock(my_bag->common_lock);
		if (calc_time(my_bag) == -1)
		{
			pthread_mutex_lock(my_bag->common_lock);
			print_task(my_bag, "", NULL);
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		if (my_bag->time_to_eat * (my_bag->type - 1) > my_bag->time_to_sleep)
			my_sleep(my_bag->time_to_eat * (my_bag->type - 1) - my_bag->time_to_sleep, my_bag);
		++(my_bag->no_of_meals);
	}
}

void	*life_cycle(void *bag)
{
	t_bag	*my_bag;

	my_bag = bag;
	gettimeofday(&(my_bag->end), NULL);
	if (my_bag->id == 5)
		ft_printf("%u\n", 200000 - time_diff(&(my_bag->end), &(my_bag->start)));
	usleep(200000 - time_diff(&(my_bag->end), &(my_bag->start)));
	gettimeofday(&(my_bag->start), NULL);
	//sleep for eat time and id
	//if (my_bag->id % my_bag->type)
		usleep(my_bag->time_to_eat * (my_bag->id % my_bag->type));
	loopy_philo(my_bag);
	return (NULL);
}