/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:43 by ayassin           #+#    #+#             */
/*   Updated: 2022/08/22 15:07:38 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	clean_exit(t_philos *stoa, t_bindle	*bindle, pthread_t	*id, int err)
{
	if (bindle)
		free(bindle);
	if (id)
		free(id);
	if (stoa && stoa->arrlock)
		free(stoa->arrlock);
	if (stoa && stoa->fork_state)
		free(stoa->fork_state);
	if (err)
	{
		if (err == 1)
			printf("Malloc faild\n");
		else
			printf("Mutex faild\n");
		exit(1);
	}
	return (0);
}

int	initbindle(t_bindle *bindle, t_philos *stoa, int num, t_timeval *start)
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
	bindle->start.tv_sec = start->tv_sec;
	bindle->start.tv_usec = start->tv_usec;
	bindle->time = 0;
	bindle->fork_state_lock1
		= &(stoa->arrlock[((num + 1) & 0xfffe) % stoa->pop]);
	bindle->fork_state_lock2
		= &(stoa->arrlock[((num & 0xfffe) + 1) % stoa->pop]);
	bindle->fork_state1 = &(stoa->fork_state[((num + 1) & 0xfffe) % stoa->pop]);
	bindle->fork_state2 = &(stoa->fork_state[((num & 0xfffe) + 1) % stoa->pop]);
	*(bindle->fork_state1) = 0;
	return (0);
}

int	mintphilos(t_philos *stoa, t_bindle *bindle, pthread_t *id)
{
	int			i;
	t_timeval	srt;

	i = -1;
	gettimeofday(&srt, NULL);
	pthread_mutex_lock(&(stoa->lock));
	while (++i < stoa->pop)
		initbindle(&(bindle[i]), stoa, i, &srt);
	i = 0;
	while (i < stoa->pop && stoa->death != 1)
	{
		if (pthread_create(&(id[i]), NULL, life_cycle2, &(bindle[i])) != 0)
		{
			if (!stoa->death)
				printf("Philosopher %i died at birth\n", i);
			stoa->death = 1;
			break ;
		}
		++i;
	}
	pthread_mutex_unlock(&(stoa->lock));
	return (i);
}

/**
 * @brief 
 * 
 * @param stoa the house that has philosophers
 * @return int not important
 */
int	creat_philos2(t_philos *stoa)
{
	int				i;
	int				pop;
	pthread_t		*id;
	t_bindle		*bindle;

	pop = stoa->pop;
	i = pthread_mutex_init(&(stoa->lock), NULL);
	stoa->arrlock = malloc(sizeof(pthread_mutex_t) * stoa->pop);
	id = (pthread_t *)malloc(sizeof(*id) * stoa->pop);
	bindle = (t_bindle *)malloc(sizeof(*bindle) * stoa->pop);
	stoa->fork_state = (int *)malloc(sizeof(int) * stoa->pop);
	if (!stoa->arrlock || !id || !bindle || !stoa->fork_state || i)
		clean_exit(stoa, bindle, id, i + 1);
	i = 0;
	while (i < pop && !pthread_mutex_init(&((stoa->arrlock)[i]), NULL))
		(stoa->fork_state)[i++] = 0;
	if (i != pop)
		clean_exit(stoa, bindle, id, 2);
	pop = mintphilos(stoa, bindle, id);
	i = 0;
	while (i < pop)
		pthread_join(id[i++], NULL);
	clean_exit(stoa, bindle, id, 0);
	return (0);
}

/**
 * @brief atoi arguments and checks if they are valid
 * 
 * @param argv no. of args
 * @param argc args
 * @return int error no
 */
int	main(int argv, char **argc)
{
	t_philos	stoa;

	if (argv < 5 || argv > 6)
		return (printf("Wrong number of arguments\n") > 0);
	stoa.pop = philo_atoi(argc[1], 1);
	stoa.time_to_die = philo_atoi(argc[2], 1000);
	stoa.time_to_eat = philo_atoi(argc[3], 1000);
	stoa.time_to_sleep = philo_atoi(argc[4], 1000);
	stoa.death = 0;
	stoa.no_of_meals = -2;
	if (argv == 6)
		stoa.no_of_meals = philo_atoi(argc[5], 1);
	// printf("%lld %lld %lld \n",stoa.time_to_die, stoa.time_to_eat, stoa.time_to_sleep);
	if (stoa.pop <= 0 || stoa.time_to_die <= 0 || stoa.no_of_meals == -1
		|| stoa.time_to_eat <= 0 || stoa.time_to_sleep <= 0)
		return (printf("Inavlid arguments\n") > 0);
	if (stoa.pop == 0)
		return (0);
	if ((stoa.time_to_die <= (2 + stoa.pop % 2) * stoa.time_to_eat
			|| (stoa.time_to_sleep + stoa.time_to_eat)
			>= stoa.time_to_die || stoa.pop <= 1) && stoa.no_of_meals)
		printf("A philosopher will die\n");
	creat_philos2(&stoa);
	return (0);
}
