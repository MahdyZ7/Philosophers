/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:43 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/23 14:51:14 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	clean_exit(char *msg)
{
	ft_printf(msg);
	exit(-1);
}

int	creat_philos(t_philos *ole)
{
	int				i;
	pthread_t		*id;
	t_bag			*my_bag;
	struct timeval	false_start;
	int				death;

	i = 0;
	death = 0;
	pthread_mutex_init(&(ole->lock), NULL);
	id = (pthread_t *)malloc(sizeof(*id) * ole->population);
	my_bag = (t_bag *)malloc(sizeof(*my_bag) * ole->population);
	gettimeofday(&false_start, NULL);
	while (i < ole->population)
	{
		my_bag[i].id = i + 1;
		my_bag[i].max_no_of_meals = ole->no_of_meals;
		my_bag[i].no_of_meals = 0;
		my_bag[i].time_to_die = ole->time_to_die;
		my_bag[i].death_clock = ole->time_to_die;
		my_bag[i].death = &death;
		my_bag[i].time_to_sleep = ole->time_to_sleep;
		my_bag[i].time_to_eat = ole->time_to_eat;
		my_bag[i].type = ole->population % 2 + 2;
		my_bag[i].common_lock = &(ole->lock);
		my_bag[i].start.tv_sec = false_start.tv_sec;
		my_bag[i].start.tv_usec = false_start.tv_usec;
		pthread_create(&(id[i]), NULL, life_cycle, &(my_bag[i]));
		ft_printf("*");
		++i;
	}
	i = 0;
	while (i < ole->population)
		pthread_join(id[i++], NULL);
	free(my_bag);
	free(id);
	return (0);
}

int	main(int argv, char **argc)
{
	t_philos	philosophers;
	int			valid_args;

	valid_args = 1;
	if (argv < 5 || argv > 6)
		clean_exit("Wrong number of arguments\n");
	philosophers.population = minitalk_atoi(argc[1], &valid_args);
	philosophers.time_to_die = minitalk_atoi(argc[2], &valid_args) * 1000;
	philosophers.time_to_eat = minitalk_atoi(argc[3], &valid_args) * 1000;
	philosophers.time_to_sleep = minitalk_atoi(argc[4], &valid_args) * 1000;
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
	else
		ft_printf("All GOOD %d %d\n", philosophers.time_to_die / philosophers.time_to_eat, philosophers.time_to_die);
	creat_philos(&philosophers);
	return (0);
}