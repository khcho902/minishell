/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 02:54:55 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "prompt.h"
# include "msh_struct.h"
# include <sys/wait.h>
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
 ** utils.c
 */
void	ft_double_free(char **ptr);
int		ft_max(int n1, int n2);

/*
 ** prompt.c
 */
void	show_logo();
void	show_prompt(t_msh *msh);
#endif
