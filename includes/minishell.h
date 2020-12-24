/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/25 01:41:34 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "prompt.h"
# include "minishell_struct.h"
# include <string.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <signal.h>

# include <stdio.h>

/*
 ** builtins/
 */
void	do_cd(t_msh *msh);
void	do_echo(t_msh *msh);
void	do_env(t_msh *msh);
void	do_exit(t_msh *msh);
void	do_export(t_msh *msh);
void	do_pwd(t_msh *msh);
void	do_unset(t_msh *msh);
void	print_env(t_msh *msh, t_dict **env_arr, int env_len, char *command);

/*
 ** executor/
 */
int		executor(t_msh *msh);
void	redirection_input_fd(t_msh *msh, t_list *list);
void	redirection_output_fd(t_msh *msh, t_list *list);
int		close_fds(t_msh *msh, pid_t pid, int pipe_open);
void	child_process(t_msh *msh, t_exe_fn func);
int		create_process(t_msh *msh, t_exe_fn func);

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
void	sanitize_token(t_list *token, t_msh *msh);

/*
 ** errors/
 */
void	exit_print_err(char *err_msg);
void	nonexit_print_err(char *err_msg);
int		print_syntax_err(char *program_name, char *token);
int		command_not_found(char *program_name, char *cmd);

/*
 ** prompt/
 */
void	show_logo();
void	show_prompt(t_msh *msh);

/*
 ** utils/
 */
void	ft_double_free(void **ptr);
int		is_in_charset(char c, char *str);
int		ft_strcmp(const char *s1, const char *s2);
t_dict	*get_env_dict(t_dict **env, char *key);
char	**ft_envjoin(t_dict **env, int env_len);
void	append_char_to_str(char **str, char c);
void	quick_sort_env(int left, int right, t_dict **env);
void	copy_env(t_msh *msh, t_dict **dst);
void	free_and_get_value(char **dst, char *src);
void	env_free(t_dict **env);

/*
 ** signal.c
 */
void	sig_handler(int signo);
void	init_signal(void);
#endif
