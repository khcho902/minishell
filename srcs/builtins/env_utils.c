/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:35:43 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 02:54:37 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		builtins(t_msh *msh)
{
	if (msh->cmd_key == k_cd)
		do_cd(msh);
	else if (msh->cmd_key == k_echo)
		do_echo(msh, STDOUT);
	else if (msh->cmd_key == k_env)
		do_env(msh->env_list, STDOUT);
	else if (msh->cmd_key == k_export)
		do_export(msh);
	else if (msh->cmd_key == k_pwd)
		do_pwd(msh, STDOUT);
	else if (msh->cmd_key == k_unset)
		do_unset(msh);
	else if (msh->cmd_key == k_exit)
		exit(0);
	return (0);
}

void	print_env(t_list *env_list, int fd)
{
	t_list	*l;
	t_dict	*dict;

	l = env_list;
	while (l)
	{
		dict = l->content;
		ft_putstr_fd(dict->key, fd);
		ft_putchar_fd('=', fd);
		ft_putstr_fd(dict->value, fd);
		ft_putchar_fd('\n', fd);
		l = l->next;
	}
}

t_dict	*dict_generator(char *env)
{
	t_dict	*dict;
	char	*chr;

	if (!(dict = (t_dict *)malloc(sizeof(t_dict))))
		return (NULL);
	chr = ft_strchr(env, '=');
	dict->key = ft_substr(env, 0, chr - env);
	dict->value = chr + 1;
	return (dict);
}

t_list	*init_env(char **env)
{
	t_list	*l;
	int		i;

	l = NULL;
	i = 0;
	while (env[i])
	{
		if (!l)
			l = ft_lstnew(dict_generator(env[i]));
		else
			ft_lstadd_back(&l, ft_lstnew(dict_generator(env[i])));
		i++;
	}
	return (l);
}
