/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/23 19:26:32 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*key_valid_test(char *key, char *chr, char *arg)
{
	int		idx;

	if (!(key = ft_substr(arg, 0, chr - arg)))
		exit_print_err(strerror(errno));
	idx = 0;
	if (!(ft_isalpha(key[idx])))
		return (NULL);
	while (key[++idx])
	{
		if (!(ft_isalnum(key[idx]) || key[idx] == '_'))
			return (NULL);
	}
	return (key);
}

int			get_export_len(t_msh *msh, char **args)
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
				new_len++;
			free(key);
		}
		idx++;
	}
	return (new_len);
}

void		export_env_oneline(t_msh *msh, t_dict **temp, char *arg, char *chr)
{
	if (!(temp[msh->env_len] = (t_dict *)malloc(sizeof(t_dict))))
		exit_print_err(strerror(errno));
	if (!(temp[msh->env_len]->key = ft_substr(arg, 0, chr - arg)))
		exit_print_err(strerror(errno));
	if (!(temp[msh->env_len]->value = ft_strdup(chr + 1)))
		exit_print_err(strerror(errno));
	msh->env_len += 1;
}

t_dict		**export_env(t_msh *msh, t_dict **temp, char **args)
{
	char	*chr;
	char	*key;
	t_dict	*dict;
	int		idx;

	idx = 0;
	while (++idx < msh->cmds->length)
		if ((chr = ft_strchr(args[idx], '=')) != NULL)
		{
			if (!(key = key_valid_test(key, chr, args[idx])))
				ft_putstr_fd("not a valid identifier\n", STDERR);
			else
			{
				if ((dict = get_env_dict(temp, key)) != NULL)
				{
					free(dict->value);
					if (!(dict->value = ft_strdup(chr + 1)))
						exit_print_err(strerror(errno));
				}
				else
					export_env_oneline(msh, temp, args[idx], chr);
			}
			free(key);
		}
	temp[msh->env_len] = NULL;
	return (temp);
}

void		do_export(t_msh *msh)
{
	t_dict	**temp;

	if (!msh->cmds->args[1])
	{
		if (!(temp = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 1))))
			exit_print_err(strerror(errno));
		temp = copy_env(msh, temp);
		quick_sort_env(0, msh->env_len - 1, temp);
		print_env(msh, temp, msh->env_len, "export");
		ft_double_free((void **)temp);
	}
	else
	{
		if (!(temp = (t_dict **)malloc(sizeof(t_dict *) *
						(get_export_len(msh, msh->cmds->args) + 1))))
			exit_print_err(strerror(errno));
		temp = copy_env(msh, temp);
		temp = export_env(msh, temp, msh->cmds->args);

		ft_double_free((void **)msh->env);
		msh->env = temp;
	}
}
