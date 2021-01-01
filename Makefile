# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/23 14:34:40 by jiseo             #+#    #+#              #
#    Updated: 2021/01/01 18:58:32 by kycho            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell
LIBFT			=	./libft/libft.a
SRCS_PATH		=	./srcs/
INCS			=	-Iincludes
RAW_SRCS		=	main.c \
					signal.c \
					\
					prompt/prompt.c \
					\
					builtins/get_builtin_executor.c \
					builtins/do_cd.c \
					builtins/do_echo.c \
					builtins/do_env.c \
					builtins/do_exit.c \
					builtins/do_export.c \
					builtins/do_pwd.c \
					builtins/do_unset.c \
					\
					executing/executing.c \
					\
					init_free_msh/init_msh.c \
					init_free_msh/free_msh_member.c \
					\
					parsing/parsing.c \
					parsing/split_token.c \
					parsing/check_token_valid.c \
					parsing/making_cmd.c \
					parsing/sanitize_token.c \
					\
					utils/ft_double_free.c \
					utils/ft_strcmp.c \
					utils/is_in_charset.c \
					utils/get_env_dict.c \
					utils/ft_envjoin.c \
					utils/quick_sort_env.c \
					utils/append_char_to_str.c \
					utils/copy_env.c \
					utils/free_and_get_value.c \
					utils/env_free.c \
					\
					errors/exit_print_err.c \
					errors/nonexit_print_err.c \
					errors/print_syntax_err.c \
					errors/command_not_found.c \

SRCS			=	$(addprefix $(SRCS_PATH), $(RAW_SRCS))
OBJS			=	$(SRCS:.c=.o)
CC				= 	gcc
CFLAGS 			=	-Wall -Wextra -Werror
LIB				=	-Llibft -lft
LIBFT			=	./libft
RM				= 	rm -f

.PHONY : all clean fclean re val r

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
	valgrind --leak-check=full --show-leak-kinds=all --error-limit=no --track-origins=yes ./minishell

r: all
	./minishell
