# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/23 14:34:40 by jiseo             #+#    #+#              #
#    Updated: 2020/11/25 22:02:44 by jiseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell
LIBFT			=	./libft/libft.a
SRCS_PATH		=	./srcs/
INCS			=	-Iincludes
RAW_SRCS		=	main.c \
					directory.c \
					env.c \
					env_functions.c \
					prompt.c \
					utils.c

SRCS			=	$(addprefix $(SRCS_PATH), $(RAW_SRCS))
OBJS			=	$(SRCS:.c=.o)
CC				= 	gcc
CFLAGS 			=	-Wall -Wextra -Werror
LIB				=	-Llibft -lft
LIBFT			=	./libft
RM				= 	rm -f

.PHONY : all clean fclean re valgrind run

all:			$(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INCS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $(<:.c=.o)

clean:
	$(MAKE) -C $(LIBFT) clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT) fclean
	$(RM) $(NAME)

re: fclean all

val:
	valgrind --leak-check=full --error-limit=no ./minishell

r: all
	./minishell
