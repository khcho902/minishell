/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/18 23:49:15 by kycho            ###   ########.fr       */
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
	if (!(key = (char *)malloc(sizeof(char) * (key_len + 1))))
		exit_print_err(strerror(errno));
	ft_strlcpy(key, str, key_len + 1);
	return (key);
}

char		*get_env_value(char *str)
{
	char	*value;
	int		key_len;

	key_len = 0;
	while (str[key_len] && str[key_len] != '=')
		key_len++;
	if (str[key_len] == '\0')
		return (NULL);
	if (!(value = ft_strdup(str + key_len + 1)))
		exit_print_err(strerror(errno));
	return (value);
}

void		do_export(t_msh *msh, t_cmd *cmd)
{
	t_dict	**sorted_env;
	char	*tmp;
	int i;
	int res;


	res = 0;
	if (cmd->length == 1)
	{
		if (!(sorted_env = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 1))))
			exit_print_err(strerror(errno));
		i = 0;
		while (i < msh->env_len)
		{
			sorted_env[i] = msh->env[i];
			i++;
		}
		sorted_env[i] = NULL;
		quick_sort_env(0, msh->env_len - 1, sorted_env);
		i = 0;
		while (i < msh->env_len)
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
			i++;
		}
		free(sorted_env);
		msh->exit_status = res;
		return ;
	}

	i = 1;
	while (i < cmd->length)
	{
		char * key = get_env_key(cmd->args[i]);
		char * value = get_env_value(cmd->args[i]);

		if(is_fine_env_key(key) == TRUE)
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
			res = 1;
		}
		free(key);
		free(value);
		i++;
	}
	msh->exit_status = res;
}
