/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 04:22:01 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/13 21:18:06 by kycho            ###   ########.fr       */
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


int			is_fine_env_key(char *key)
{
	int i;

	if (ft_strlen(key) == 0)
		return (FALSE);
	if (!(key[0] == '_' || ('a' <= key[0] && key[0] <= 'z') ||
				('A' <= key[0] && key[0] <= 'Z')))
		return (FALSE);
	i = 0;
	while (key[i])
	{
		if (!((key[i] == '_') || ('a' <= key[i] && key[i] <= 'z') ||
			('A' <= key[i] && key[i] <= 'Z') ||
			('0' <= key[i] && key[i] <= '9')))
			return (FALSE);
		i++;
	}
	return (TRUE);
}


char		*get_env_key(char *str)
{
	char	*key;
	int		key_len;

	key_len = 0;
	while (str[key_len] && str[key_len] != '=')
		key_len++;

//	dprintf(2, "in get_env_key key_len : %d\n", key_len);

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
	
//	dprintf(2, "in get_env_value key_len : %d\n", key_len);
//	dprintf(2, "in get_env_value   value : %s\n", str + key_len + 1);

	if (str[key_len] == '\0')
		return (NULL);
	if (!(value = ft_strdup(str + key_len + 1)))
		exit_print_err(strerror(errno));
	return (value);
}

void		do_export(t_msh *msh, t_cmd *cmd)
{

//	write(1, "in do_export\n", 13);

	/*
	for(int k = 0; k < msh->env_len; k++){
		dprintf(2, "|%s| |%s|\n", msh->env[k]->key, msh->env[k]->value);

	}
	*/
	t_dict	**temp;
	char	*tmp_str;
	int i;

	if (cmd->length == 1)
	{
		if (!(temp = (t_dict **)malloc(sizeof(t_dict *) * (msh->env_len + 1))))
			exit_print_err(strerror(errno));
	//	copy_env(msh, temp);
		i = 0;
		while (i < msh->env_len)
		{
			temp[i] = msh->env[i];
			i++;
		}
		temp[i] = NULL;

		quick_sort_env(0, msh->env_len - 1, temp);

	//	print_env(msh, temp, msh->env_len, "export");
		i = 0;
		while (i < msh->env_len)
		{
			ft_putstr_fd("declare -x ", STDOUT);
			ft_putstr_fd(temp[i]->key, STDOUT);
			if (temp[i]->value != NULL)
			{
				ft_putstr_fd("=\"", STDOUT);
				tmp_str = insert_char_before_set(temp[i]->value, "\"\\$", '\\');
				ft_putstr_fd(tmp_str, STDOUT);
				ft_putchar_fd('"', STDOUT);
				free(tmp_str);
			}
			ft_putchar_fd('\n', STDOUT);
			i++;
		}

	//	env_free(temp);
		free(temp);
		return ;
	}
	/*
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
	*/


	i = 1;
	while (i < cmd->length)
	{
		char * key = get_env_key(cmd->args[i]);
		char * value = get_env_value(cmd->args[i]);

//		dprintf(2, "key : |%s|\n", key);
//		dprintf(2, "value : |%s|\n", value);
		if(is_fine_env_key(key) == TRUE)
		{
			set_env_dict(msh, key, value);
		}
		else
		{
			dprintf(2, "minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
			msh->exit_status = 1;
		}
		
		free(key);
		free(value);
		i++;
	}





}
