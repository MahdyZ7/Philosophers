# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/22 13:02:34 by ayassin           #+#    #+#              #
#    Updated: 2022/08/23 16:08:33 by ayassin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

HEADERS = philosophers.h

SRC = philosophers.c philosophers_utils.c philo_life2.c

CC = gcc

CFLAGS = -Wall -Werror -Wextra -pthread

OBJS = $(SRC:.c=.o)

all : $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re
