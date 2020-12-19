/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:48:11 by kycho             #+#    #+#             */
/*   Updated: 2020/12/19 11:53:33 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_msh_env(t_msh *msh, char **env)
{
	int i;
	int	key_len;

	msh->env_len = 0;
	while (env[msh->env_len])
		msh->env_len++;
	if (!(msh->env = (t_dict**)malloc(sizeof(t_dict *) * (msh->env_len + 1))))
		exit_print_err(strerror(errno));
	msh->env[msh->env_len] = NULL;
	i = 0;
	while (env[i])
	{
		key_len = ft_strchr(env[i], '=') - env[i];
		if (!(msh->env[i] = (t_dict *)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(msh->env[i]->key = (char *)malloc(sizeof(char) * (key_len + 1))))
			exit_print_err(strerror(errno));
		ft_strlcpy(msh->env[i]->key, env[i], key_len + 1);
		if (!(msh->env[i]->value = ft_strdup(env[i] + key_len + 1)))
			exit_print_err(strerror(errno));
		i++;
	}
	show_logo();
}

void	init_msh_path(t_msh *msh)
{
	int		i;

	i = 0;
	while (msh->env[i])
	{
		if (!ft_strcmp(msh->env[i]->key, "PATH"))
		{
			msh->path = ft_split(msh->env[i]->value, ':');
			if (msh->path == NULL)
				exit_print_err(strerror(errno));
			break ;
		}
		i++;
	}
}

void	init_msh(char *program_name, t_msh *msh, char **env)
{
	int i;

	i = ft_strlen(program_name) - 1;
	while (i >= 0 && program_name[i] != '/')
		i--;
	if (!(msh->program_name = ft_strdup(program_name + i + 1)))
		exit_print_err(strerror(errno));
	msh->exit_status = 0;
	msh->tokens = NULL;
	msh->cmds = NULL;
	init_msh_env(msh, env);
	init_msh_path(msh);
}
