/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 07:08:23 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "prompt.h"
# include "msh_struct.h"
# include <string.h>
# include <sys/wait.h>
# include <sys/errno.h>

# include <stdio.h>

/*
 ** builtins/
 */
t_dict	*dict_generator(char *env);
t_list	*init_env(char **env);
void	print_env(t_list *env_list, int fd);
int		builtins(t_msh *msh);
void	do_cd(t_msh *msh);
void	do_echo(t_msh *msh, int fd);
void	do_env(t_list *env_list, int fd);
void	do_export(t_msh *msh);
void	do_pwd(t_msh *msh, int fd);
void	do_unset(t_msh *msh);

/*
 ** init.c
 */
int		builtin_compare(char *str);
void	exec_process(t_msh *msh);
void	create_process(t_msh *msh);
void	main_loop(t_msh *msh);
void	init_main(t_msh *msh, char **env);

/*
 ** parsing.c
 */
void	split_token_sub(char *input, int *i, int *len);
void	split_token(char *input, t_list **tokens, int i);
int		check_token_valid(char *program_name, t_list *now);
t_cmd	*get_new_cmd(t_cmd *previous);
void	add_args(t_cmd *cmd, char *token_content);
void	add_redirection_file(t_cmd *cmd, t_list **token);
void	making_cmd(t_msh *msh);
int		parsing(t_msh *msh, char *input);
void	init_msh_env(t_msh *msh, char **env);
void	init_msh_path(t_msh *msh);
void	init_msh(char *program_name, t_msh *msh, char **env);

/*
 ** error.c
 */
void	exit_print_err(char *err_msg1, char *err_msg2, int exit_status);
int		print_syntax_err(char *program_name, char *token);

/*
 ** prompt.c
 */
void	show_logo();
void	show_prompt(t_msh *msh);

/*
 ** utils.c
 */
void	ft_double_free(char **ptr);
int		is_in_charset(char c, char *str);
#endif
