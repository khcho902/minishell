/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/21 05:40:17 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_env_key(char *str)
{
	char	*key;
	int		key_len;

	key_len = 0;
	while (str[key_len] && str[key_len] != '=')
		key_len++;
	if (key_len > 0 && str[key_len] == '=' && str[key_len - 1] == '+')
		key_len--;
	if (!(key = (char *)malloc(sizeof(char) * (key_len + 1))))
		exit_print_err(strerror(errno));
	ft_strlcpy(key, str, key_len + 1);
	return (key);
}

char		*get_env_value(char *str, char *key, t_msh *msh)
{
	char	*value;
	char	*tmp;
	t_dict	*env_dict;

	if (str[ft_strlen(key)] == '\0')
		return (NULL);
	if (str[ft_strlen(key)] == '=')
	{
		if (!(value = ft_strdup(str + ft_strlen(key) + 1)))
			exit_print_err(strerror(errno));
	}
	else
	{
		tmp = "";
		if (ft_strcmp(key, "PATH") == 0 && msh->path != NULL)
			tmp = msh->path;
		else if ((env_dict = get_env_dict(msh->env, key)) != NULL &&
				env_dict->value != NULL)
			tmp = env_dict->value;
		if (!(value = ft_strjoin(tmp, str + ft_strlen(key) + 2)))
			exit_print_err(strerror(errno));
	}
	return (value);
}

void		export_print(t_msh *msh, int i)
{
	char	*tmp;
	t_dict	**sorted_env;

	if (!(sorted_env = malloc(sizeof(t_dict *) * (msh->env_len + 1))))
		exit_print_err(strerror(errno));
	while (++i < msh->env_len)
		sorted_env[i] = msh->env[i];
	sorted_env[i] = NULL;
	quick_sort_env(0, msh->env_len - 1, sorted_env);
	i = -1;
	while (++i < msh->env_len)
	{
		ft_putstr_fd("declare -x ", STDOUT);
		ft_putstr_fd(sorted_env[i]->key, STDOUT);
		if (sorted_env[i]->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT);
			tmp = insert_char_before_set(sorted_env[i]->value, "\"\\$", '\\');
			ft_putstr_fd(tmp, STDOUT);
			ft_putchar_fd('"', STDOUT);
			free(tmp);
		}
		ft_putchar_fd('\n', STDOUT);
	}
	free(sorted_env);
}

void		export_env_set(t_msh *msh, t_cmd *cmd, int i, int *res)
{
	char	*key;
	char	*value;

	key = get_env_key(cmd->args[i]);
	value = get_env_value(cmd->args[i], key, msh);
	if (is_fine_env_key(key) == TRUE)
	{
		free(msh->lstcmd);
		if (!(msh->lstcmd = ft_strdup(key)))
			exit_print_err(strerror(errno));
		if (ft_strcmp(key, "PATH") == 0)
		{
			free(msh->path);
			if (!(msh->path = ft_strdup(value)))
				exit_print_err(strerror(errno));
		}
		set_env_dict(msh, key, value);
	}
	else
	{
		print_identifier_err(msh->program_name, cmd->args[0], cmd->args[i]);
		*res = 1;
	}
	free(key);
	free(value);
}

void		do_export(t_msh *msh, t_cmd *cmd)
{
	int res;
	int i;

	if (cmd->length == 1)
	{
		export_print(msh, -1);
		msh->exit_status = 0;
		return ;
	}
	res = 0;
	i = 1;
	while (i < cmd->length)
	{
		export_env_set(msh, cmd, i, &res);
		i++;
	}
	msh->exit_status = res;
}
