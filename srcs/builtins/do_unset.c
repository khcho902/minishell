/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:52 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/12 15:17:15 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	free_dict(t_dict *dict)
{
	free(dict->key);
	free(dict->value);
	free(dict);
}

void	unset_env_dict(t_msh *msh, char *key)
{
	t_dict	*dict;
	t_dict	**new_env;
	int		i;
	int		new_i;

	dict = get_env_dict(msh->env, key);
	if (dict == NULL)
		return ;
	if (!(new_env = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len))))
		exit_print_err(strerror(errno));
	new_env[msh->env_len - 1] = NULL;
	i = 0;
	new_i = 0;
	while (i < msh->env_len)
	{
		if (ft_strcmp(msh->env[i]->key, key) == 0)
			free_dict(msh->env[i]);
		else
			new_env[new_i++] = msh->env[i];
		i++;
	}
	msh->env_len--;
	free(msh->env);
	msh->env = new_env;
}
*/
/*
int		get_unset_len(t_msh *msh, char **args)
{
	int		idx;
	int		new_len;
	char	*chr;
	char	*key;

	idx = 1;
	new_len = msh->env_len;
	while (idx < msh->cmds->length)
	{
		if ((chr = ft_strchr(args[idx], '=')) != NULL)
		{
			if (!(key = ft_substr(args[idx], 0, chr - args[idx])))
				exit_print_err(strerror(errno));
			if (get_env_dict(msh->env, key) == NULL)
				new_len--;
			free(key);
		}
		idx++;
	}
	return (new_len);
}

int		check_args(char *key, char **args, int args_len)
{
	int		idx;

	idx = 1;
	while (idx < args_len)
	{
		if (!(ft_strcmp(key, args[idx])))
			return (0);
		idx++;
	}
	return (1);
}
*/

void	do_unset(t_msh *msh, t_cmd *cmd)
{
	int i;

	i = 1;
	while (i < cmd->length)
	{
		if (ft_strcmp(cmd->args[i], "PWD") == 0)
		{
			free(msh->pwd);
			if (!(msh->pwd = ft_strdup("")))
				exit_print_err(strerror(errno));
		}
		else if (ft_strcmp(cmd->args[i], "OLDPWD") == 0)
		{
			free(msh->oldpwd);
			if (!(msh->oldpwd = ft_strdup("")))
				exit_print_err(strerror(errno));
		}
		unset_env_dict(msh, cmd->args[i]);
		i++;
	}

	/*
	t_dict		**temp;
	int			idx;
	int			temp_idx;

	if (!(temp = (t_dict **)malloc(sizeof(t_dict *) *
					(get_unset_len(msh, cmd->args) + 1))))
		exit_print_err(strerror(errno));
	idx = -1;
	temp_idx = 0;
	while (++idx < msh->env_len)
	{
		if (check_args(msh->env[idx]->key, cmd->args, cmd->length))
		{
			if (!(temp[temp_idx] = (t_dict *)malloc(sizeof(t_dict))))
				exit_print_err(strerror(errno));
			if (!(temp[temp_idx]->key = ft_strdup(msh->env[idx]->key)))
				exit_print_err(strerror(errno));
			if (msh->env[idx]->value == NULL)
			{
				temp[temp_idx]->value = NULL;
			}	
			else
			{
				if (!(temp[temp_idx]->value = ft_strdup(msh->env[idx]->value)))
					exit_print_err(strerror(errno));
			}
			temp_idx++;
		}
	}
	temp[temp_idx] = NULL;
	env_free(msh->env);
	msh->env_len = temp_idx;
	msh->env = temp;
	*/
}
