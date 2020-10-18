# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/28 01:38:31 by jiseo             #+#    #+#              #
#    Updated: 2020/08/09 02:06:28 by jiseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS_B = $(SRCS_B:.c=.o)
LIBC = ar rcs
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
INCS = .

.PHONY : all clean fclean re .c.o

$(NAME) : ${OBJS}
	${LIBC} $(NAME) $(OBJS)

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I${INCS}

all: $(NAME)

clean:
	$(RM) $(OBJS) $(OBJS_B)

fclean: clean
	$(RM) $(NAME)

re: fclean all
