/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/12 17:14:46 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_msh *msh, t_dict **env_arr, int env_len, char *command)
{
	char	**temp;
	int		idx;

	if (!env_arr)
	{
		print_execute_err(msh->program_name, command, "command not found");
//		command_not_found(msh->program_name, command);

		return ;
	}
	idx = 0;
	temp = ft_envjoin(env_arr, env_len);
	if (!temp)
		exit_print_err(strerror(errno));
	while (temp[idx])
	{
		ft_putstr_fd(temp[idx], STDOUT);
		ft_putchar_fd('\n', STDOUT);
		idx++;
	}
	ft_double_free((void **)temp);
}

char		*key_valid_test(char *key, char *chr, char *arg)
{
	int			idx;
	const char	*reserved_variable[] = {"TERM_SESSION_ID", "SSH_AUTH_SOCK",
	"LC_TERMINAL_VERSION", "Apple_PubSub_Socket_Render", "COLORFGBG",
	"ITERM_PROFILE", "XPC_FLAGS", "LANG", "PWD", "SHELL", "TERM_PROGRAM",
	"TERM_PROGRAM_VERSION", "PATH", "LC_TERMINAL", "COLORTERM", "TERM", "HOME",
	"TMPDIR", "USER", "XPC_SERVICE_NAME", "LOGNAME", "ITERM_SESSION_ID",
	"__CF_USER_TEXT_ENCODING", "SHLVL", "OLDPWD", NULL};

	if (!(key = ft_substr(arg, 0, chr - arg)))
		exit_print_err(strerror(errno));
	idx = 0;
	if (!(ft_isalpha(key[idx])))
		return (NULL);
	while (key[++idx])
		if (!(ft_isalnum(key[idx]) || key[idx] == '_'))
			return (NULL);
	idx = 0;
	while (reserved_variable[idx])
	{
		if (!(ft_strcmp(reserved_variable[idx], key)))
			return (NULL);
		idx++;
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

void		export_env(t_msh *msh, t_dict **temp, char **args)
{
	char	*chr;
	char	*key;
	t_dict	*dict;
	int		idx;

	idx = 0;
	dict = NULL;
	while (idx < msh->cmds->length)
	{
		if ((chr = ft_strchr(args[idx], '=')) != NULL)
		{
			if (!(key = key_valid_test(key, chr, args[idx])))
				ft_putstr_fd("not a valid identifier\n", STDERR);
			else
			{
				if ((dict = get_env_dict(temp, key)) != NULL)
					free_and_get_value(&(dict->value), chr + 1);
				else
					export_env_oneline(msh, temp, args[idx], chr);
			}
			free(key);
		}
		idx++;
	}
}

void		do_export(t_msh *msh, t_cmd *cmd)
{

	//write(1, "in do_export\n", 13);
	t_dict	**temp;

	if (cmd->args[1] == NULL)
	{
		if (!(temp = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 1))))
			exit_print_err(strerror(errno));
		copy_env(msh, temp);
		quick_sort_env(0, msh->env_len - 1, temp);
		print_env(msh, temp, msh->env_len, "export");
		env_free(temp);
		//free(temp);
	}
	else
	{
		if (!(temp = (t_dict **)malloc(sizeof(t_dict *) *
						(get_export_len(msh, cmd->args) + 1))))
			exit_print_err(strerror(errno));
		copy_env(msh, temp);
		export_env(msh, temp, cmd->args);
		temp[msh->env_len] = NULL;
		env_free(msh->env);
		msh->env = temp;
	}
}
