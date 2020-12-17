/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/17 15:08:11 by jiseo            ###   ########.fr       */
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
# include <string.h>
# include <sys/errno.h>

/*
 ** builtins/
 */
void	do_cd(t_msh *msh);
void	do_echo(t_msh *msh, int fd);
void	do_env(t_dict **env_arr, int fd);
void	do_export(t_msh *msh, int fd);
void	do_pwd(t_msh *msh, int fd);
void	do_unset(t_msh *msh);
int		builtins(t_msh *msh, int cmd_key);

/*
 ** init.c
 */
int		builtin_compare(char *str);
void	exec_process(t_msh *msh, t_cmd *cmd, char **av, char **env);
void	executor(t_msh *msh, t_cmd *cmd);

/*
 ** init_free_msh/
 */
void	init_msh(char *program_name, t_msh *msh, char **env);
void	free_msh_member(t_msh *msh);

/*
 ** parsing/
 */
int		parsing(t_msh *msh, char *input);
void	split_token(char *input, t_list **tokens, int i);
int		check_token_valid(char *program_name, t_list *now);
void	making_cmd(t_msh *msh);

/*
 ** errors/
 */
void	exit_print_err(char *err_msg);
int		print_syntax_err(char *program_name, char *token);
void	command_not_found(char *str);

/*
 ** prompt.c
 */
void	show_logo();
void	show_prompt(t_msh *msh);

/*
 ** utils/
 */
void	ft_double_free(void **ptr);
int		is_in_charset(char c, char *str);
int		ft_strcmp(const char *s1, const char *s2);
#endif
