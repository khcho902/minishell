/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/20 03:00:54 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

typedef struct	s_msh
{
	t_list		*env_list;
	char		**cmd_list;
	int			cmd_idx;
	int			cmd_key;
}				t_msh;

typedef struct	s_key_value
{
	char		*key;
	char		*value;
}				t_kv;

enum
{
	k_echo = 1,
	k_cd = 2,
	k_pwd = 3,
	k_export = 4,
	k_unset = 5,
	k_env = 6,
	k_exit = 7
};

t_kv			*key_value_generator(char *env);
t_list			*init_env(char **env);
void			do_env(t_list *l, int fd);

void			ft_double_free(char **ptr);

#endif
