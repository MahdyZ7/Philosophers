/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:43 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/23 17:11:57 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// int	check_args(int argv, char **argc)
// {
//	
// }

int	clean_exit(char *msg)
{
	ft_printf(msg);
	exit(-1);
}

int	initbindle(t_bindle *bindle, t_philos *stoa, int num, t_timeval false_start)
{
	bindle->id = num + 1;
	bindle->max_meals = stoa->no_of_meals;
	bindle->meals = 0;
	bindle->die_time = stoa->time_to_die;
	bindle->countdown = stoa->time_to_die;
	bindle->death = &(stoa->death);
	bindle->sleep_time = stoa->time_to_sleep;
	bindle->eat_time = stoa->time_to_eat;
	bindle->type = stoa->pop % 2 + 2;
	bindle->common_lock = &(stoa->lock);
	bindle->start.tv_sec = false_start.tv_sec;
	bindle->start.tv_usec = false_start.tv_usec;
	bindle->time = 0;
	//ft_printf("I am philo %d and num am resposible for %d and %d\n", num, ((num + 1) & 0xfffe) % stoa->pop, ((num & 0xfffe) + 1) % stoa->pop);
	bindle->fork_state_lock1 = &(stoa->arrlock[((num + 1) & 0xfffe) % stoa->pop]);
	bindle->fork_state_lock2 = &(stoa->arrlock[((num & 0xfffe) + 1) % stoa->pop]);
	bindle->fork_state1 = &(stoa->fork_state[((num + 1) & 0xfffe) % stoa->pop]);
	bindle->fork_state2 = &(stoa->fork_state[((num & 0xfffe) + 1) % stoa->pop]);
	*(bindle->fork_state1) = 0;
	pthread_mutex_init(bindle->fork_state_lock1, NULL);
	return (0);
}

int	creat_philos2(t_philos *stoa)
{
	int				i;
	pthread_t		*id;
	t_bindle		*bindle;
	struct timeval	false_start;
	int				death;

	i = -1;
	death = 0;
	pthread_mutex_init(&(stoa->lock), NULL);
	stoa->arrlock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * stoa->pop);
	id = (pthread_t *)malloc(sizeof(*id) * stoa->pop);
	bindle = (t_bindle *)malloc(sizeof(*bindle) * stoa->pop);
	stoa->fork_state = (int *)malloc(sizeof(int) * stoa->pop);
	gettimeofday(&false_start, NULL);
	while (++i < stoa->pop)
	{
		initbindle(&(bindle[i]), stoa, i, false_start);
		pthread_create(&(id[i]), NULL, life_cycle2, &(bindle[i]));
	}
	i = 0;
	while (i < stoa->pop)
		pthread_join(id[i++], NULL);
	free(bindle);
	free(id);
	free(stoa->arrlock);
	free(stoa->fork_state);
	return (0);
}

int	main(int argv, char **argc)
{
	t_philos	stoa;
	int			valid_args;

	valid_args = 1;
	if (argv < 5 || argv > 6)
		clean_exit("Wrong number of arguments\n");
	stoa.pop = minitalk_atoi(argc[1], &valid_args);
	stoa.time_to_die = minitalk_atoi(argc[2], &valid_args) * 1000;
	stoa.time_to_eat = minitalk_atoi(argc[3], &valid_args) * 1000;
	stoa.time_to_sleep = minitalk_atoi(argc[4], &valid_args) * 1000;
	stoa.death = 0;
	if (argv == 6)
		stoa.no_of_meals = minitalk_atoi(argc[5], &valid_args);
	else
		stoa.no_of_meals = -1;
	if (valid_args == 0)
		clean_exit("Inavlid arguments\n");
	if (stoa.time_to_die / stoa.time_to_eat < (2 + stoa.pop % 2)
		|| (stoa.time_to_sleep + stoa.time_to_eat)
		> stoa.time_to_die || stoa.pop < 2)
		ft_printf("A philosopher will definitly die\n");
	creat_philos2(&stoa);
	return (0);
}