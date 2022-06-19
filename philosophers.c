/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:43 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/19 16:26:03 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mock_pilosophers(t_philos *philos)
{
	int				i;
	int				meal_count;
	int				ittration_no;
	struct timeval	start, end;

	meal_count = 0;
	ittration_no = 0;
	gettimeofday(&start, NULL);
	while (philos->no_of_meals == -1 || meal_count < philos->no_of_meals)
	{
		ittration_no = 0;
		while (ittration_no < (2 + philos->population % 2))
		{
			i = ittration_no;
			while (i < philos->population)
			{
				gettimeofday(&end, NULL);
				printf ("%u %d is eating\n", time_diff(&end, &start), i);
				i = i + 2 + (philos->population % 2);
			}
			usleep(philos->time_to_eat * 1000);
			++ittration_no;
		}
		++meal_count;
	}
}

// int	check_args(int argv, char **argc)
// {
//	
// }

int	clean_exit(char *msg)
{
	ft_printf(msg);
	exit(-1);
}

void	*life_cycle(void *bag)
{
	t_bag	*my_bag;

	my_bag = bag;
	get_time(my_bag->end);
	usleep(200000 - time_diff(&(my_bag->end), &(my_bag->start)));
	get_time(my_bag->start);
	//sleep for eat time and id
	//loopy philo
}

int	creat_philos(t_philos *ole)
{
	int				i;
	int				*id;
	t_bag			*my_bag;
	struct timeval	false_start;

	i = 0;
	pthread_mutex_init(&(ole->lock), NULL);
	id = (int *)malloc(sizeof(int) * ole->population);
	my_bag = (t_bag *)malloc(sizeof(*my_bag) * ole->population);
	gettimeofday(&false_start, NULL);
	while (i < ole->population)
	{
		id[i] = i + 1;
		my_bag[i].no_of_meals = ole->no_of_meals;
		my_bag[i].time_to_die = ole->time_to_die;
		my_bag[i].time_to_sleep = ole->time_to_sleep;
		my_bag[i].time_to_eat = ole->time_to_eat;
		my_bag[i].odd = ole->population % 2;
		my_bag[i].common_lock = &(ole->lock);
		my_bag[i].start.tv_sec = false_start.tv_sec;
		my_bag[i].start.tv_usec = false_start.tv_usec;
		pthread_create(&(id[i]), NULL, life_cycle, &(my_bag[i]));
	}
}

int	main(int argv, char **argc)
{
	t_philos	philosophers;
	int			valid_args;
	int			i;

	valid_args = 1;
	if (argv < 5 || argv > 6)
		clean_exit("Wrong number of arguments\n");
	philosophers.population = minitalk_atoi(argc[1], &valid_args);
	philosophers.time_to_die = minitalk_atoi(argc[2], &valid_args);
	philosophers.time_to_eat = minitalk_atoi(argc[3], &valid_args);
	philosophers.time_to_sleep = minitalk_atoi(argc[4], &valid_args);
	if (argv == 6)
		philosophers.no_of_meals = minitalk_atoi(argc[5], &valid_args);
	else
		philosophers.no_of_meals = -1;
	if (valid_args == 0)
		clean_exit("Inavlid arguments\n");
	if (philosophers.time_to_die / philosophers.time_to_eat
		< (2 + philosophers.population % 2)
		|| (philosophers.time_to_sleep + philosophers.time_to_eat)
		> philosophers.time_to_die || philosophers.population < 2)
		ft_printf("A philosopher will definitly die\n");
	// mock_pilosophers(&philosophers);
	while (i < philosophers.population)
	{
		pthread_creat()
	}
	return (0);
}