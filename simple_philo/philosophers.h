/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:22:40 by ayassin           #+#    #+#             */
/*   Updated: 2022/06/23 14:54:27 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include "libft/libft.h"
# include "ft_printf/ft_printf.h"

# ifndef PRINTF_COLORS
#  define BLACK "\033[0;30m"
#  define RED "\033[0;31m"
#  define GREEN "\033[0;32m"
#  define YELLOW "\033[0;33m"
#  define BLUE "\033[0;34m"
#  define PURPLE "\033[0;35m"
#  define CYAN "\033[0;36m"
#  define WHITE "\033[0;37m"
#  define RESET_COLOR "\033[0m"
# endif

typedef struct s_philos
{
	int				population;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				no_of_meals;
	int				*fork_state;
	pthread_mutex_t	lock;
	pthread_mutex_t	*array_lock;

}				t_philos;

typedef struct s_bag
{
	int				id;
	long			death_clock;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_no_of_meals;
	int				no_of_meals;
	int				type;
	long			time;
	int				*death;
	struct timeval	start;
	struct timeval	end;
	pthread_mutex_t	*common_lock;
}				t_bag;

int		time_diff(struct timeval *end, struct timeval *start);
int		minitalk_atoi(char *str, int *valid_flag);
int		print_task(t_bag *bag, char *task, char *color);

void	*life_cycle(void *bag);

#endif