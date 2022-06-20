/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:45:04 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/20 18:49:24 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	calc_time(t_bag *my_bag)
{
	gettimeofday(&(my_bag->end), NULL);
	my_bag->time = time_diff(&(my_bag->end), &(my_bag->start));
	if (my_bag->time * 1000 > my_bag->death_clock)
		my_bag->time *= -1;
	return (0);
}

void	loopy_philo(t_bag *my_bag)
{
	while (my_bag->no_of_meals < my_bag->max_no_of_meals
		|| my_bag->max_no_of_meals == -1)
	{
		calc_time(my_bag);
		pthread_mutex_lock(my_bag->common_lock);
		// print_task(my_bag, "has taken a fork", GREEN);
		// print_task(my_bag, "has taken a fork", GREEN);
		if (print_task(my_bag, "is_eating", GREEN))
		{
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		pthread_mutex_unlock(my_bag->common_lock);
		usleep(my_bag->time_to_eat);
		my_bag->death_clock += my_bag->time_to_die; //position is uncertian
		
		//--------------
		calc_time(my_bag);
		pthread_mutex_lock(my_bag->common_lock);
		if (print_task(my_bag, "is sleeping", CYAN))
		{
			pthread_mutex_unlock(my_bag->common_lock);
			break ;
		}
		pthread_mutex_unlock(my_bag->common_lock);
		usleep(my_bag->time_to_sleep);
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
		// if (my_bag->time_to_eat * (my_bag->type - 1) > my_bag->time)
		// {
		// 	//die
		// }
		//usleep(my_bag->time_to_die - my_bag->time_to_eat * (my_bag->type - 1) - my_bag->time);
		break;
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