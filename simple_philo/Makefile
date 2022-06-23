# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/22 13:02:34 by ayassin           #+#    #+#              #
#    Updated: 2022/06/23 14:50:26 by ayassin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

HEADERS = philosophers.h

SRC = philosophers.c philosophers_utils.c philo_life.c

SUBDIRS = libft ft_printf

CC = gcc

CFLAGS = -Wall -Werror -Wextra -pthread

LINKS = -Llibft -lft -Lft_printf -lftprintf

OBJS = $(SRC:.c=.o)

all : $(NAME)

.c.o:
	for dir in $(SUBDIRS); do \
        $(MAKE) all -C $$dir; \
    done
	$(CC) $(CFLAGS) -Ilibft -Ift_printf -c $^ -o $@

$(NAME) : $(OBJS)
	for dir in $(SUBDIRS); do \
        $(MAKE) all -C $$dir; \
    done
	$(CC) $(CFLAGS) $(LINKS) $(SRC) -o $(NAME)

clean:
	rm -f $(OBJS)
	for dir in $(SUBDIRS); do \
        $(MAKE) clean -C $$dir; \
    done

fclean: clean
	rm -f $(NAME)
	for dir in $(SUBDIRS); do \
        $(MAKE) fclean -C $$dir; \
    done

re : fclean all

.PHONY: clean fclean all re
