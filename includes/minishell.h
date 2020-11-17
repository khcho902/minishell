/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/11/17 19:10:07 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

typedef struct	s_key_value
{
	char		*key;
	char		*value;
}				t_kv;

enum
{
	k_echo = 0,
	k_cd = 1,
	k_pwd = 2,
	k_export = 3,
	k_unset = 4,
	k_env = 5,
	k_exit = 6
};

t_kv			*key_value_generator(char *env);
t_list			*init_env(char **env);
void			do_env(t_list *l);

void			ft_double_free(char **ptr);

#endif
