/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:40 by ayassin           #+#    #+#             */
/*   Updated: 2022/08/23 12:20:12 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

# ifndef PRINTF_COLORS
#  define BLACK "\033[0;30m"
#  define RED "\033[0;31m"
#  define GREEN "\033[0;32m"
#  define YELLOW "\033[0;33m"
#  define BLUE "\033[0;34m"
#  define PURPLE "\033[0;35m"
#  define CYAN "\033[0;36m"
#  define WHITE "\033[0;37m"
#  define RST_COLOR "\033[0m"
# endif

typedef struct timeval	t_timeval;
typedef struct s_philos
{
	int				pop;
	int_fast64_t	time_to_die;
	int_fast64_t	time_to_eat;
	int_fast64_t	time_to_sleep;
	int				no_of_meals;
	int				death;
	int				*fork_state;
	pthread_mutex_t	lock;
	pthread_mutex_t	*arrlock;

}				t_philos;
typedef struct s_bindle
{
	int				*fork_state1;
	int				*fork_state2;
	int				id;
	int_fast64_t	countdown;
	int_fast64_t	die_time;
	int_fast64_t	eat_time;
	long			sleep_time;
	int				max_meals;
	int				meals;
	int				type;
	long long		time;
	int				*death;
	struct timeval	start;
	struct timeval	end;
	pthread_mutex_t	*fork_state_lock1;
	pthread_mutex_t	*fork_state_lock2;
	pthread_mutex_t	*common_lock;
}				t_bindle;

long long	time_diff(struct timeval *end, struct timeval *start);
long		philo_atoi(char *str, int factor);
int			print_task2(t_bindle *bag, char *task, char *color);
int			calc_time(t_bindle *bindle);
int			my_sleep2(int time_to_waste, t_bindle *bindle, t_timeval start);

void	*life_cycle2(void *bindle);

#endif