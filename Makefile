# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/23 14:34:40 by jiseo             #+#    #+#              #
#    Updated: 2020/12/19 07:44:18 by jiseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell
LIBFT			=	./libft/libft.a
SRCS_PATH		=	./srcs/
INCS			=	-Iincludes
RAW_SRCS		=	main.c \
					\
					prompt/prompt.c \
					\
					builtins/do_cd.c \
					builtins/do_echo.c \
					builtins/do_env.c \
					builtins/do_exit.c \
					builtins/do_export.c \
					builtins/do_pwd.c \
					builtins/do_unset.c \
					\
					executor/executor.c \
					\
					init_free_msh/init_msh.c \
					init_free_msh/free_msh_member.c \
					\
					parsing/parsing.c \
					parsing/split_token.c \
					parsing/check_token_valid.c \
					parsing/making_cmd.c \
					\
					utils/ft_double_free.c \
					utils/ft_strcmp.c \
					utils/is_in_charset.c \
					utils/get_env_dict.c \
					utils/ft_envjoin.c \
					utils/quick_sort_env.c \
					\
					errors/exit_print_err.c \
					errors/print_syntax_err.c \
					errors/command_not_found.c \
					errors/print_err.c \

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
