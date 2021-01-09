/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:48:11 by kycho             #+#    #+#             */
/*   Updated: 2021/01/10 00:21:33 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_dict(t_msh *msh, char *key, char *value)
{
	t_dict	*dict;
	t_dict	**new_env;
	int		i;

	if (!(dict = (t_dict *)malloc(sizeof(t_dict) * 1)))
		exit_print_err(strerror(errno));
	if (!(dict->key = ft_strdup(key)))
		exit_print_err(strerror(errno));
	if (!(dict->value = ft_strdup(value)))
		exit_print_err(strerror(errno));
	if (!(new_env = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 2))))
		exit_print_err(strerror(errno));
	i = 0;
	while (i < msh->env_len)
	{
		new_env[i] = msh->env[i];
		i++;
	}
	new_env[msh->env_len] = dict;
	new_env[msh->env_len + 1] = NULL;
	msh->env_len++;
	free(msh->env);
	msh->env = new_env;
}

void	set_env_dict(t_msh *msh, char *key, char *value)
{
	t_dict *dict;

	dict = get_env_dict(msh->env, key);
	if (dict != NULL)
	{
		free(dict->value);
		if (!(dict->value = ft_strdup(value)))
			exit_print_err(strerror(errno));
	}
	else
		add_env_dict(msh, key, value);
}

void	init_pwd_env(t_msh *msh)
{
	char	*value;

	if (!(value = getcwd(NULL, 0)))
		exit_print_err(strerror(errno));
	set_env_dict(msh, "PWD", value);
	free(value);
}


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
	init_pwd_env(msh);
//	init_shlvl_env(msh);
}

void	init_msh_path(t_msh *msh)
{
	int		i;

	msh->path = NULL;
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
	if (msh->path == NULL)
	{
		msh->path = (char **)malloc(sizeof(char *));
		if (msh->path == NULL)
			exit_print_err(strerror(errno));
		msh->path[0] = NULL;
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
