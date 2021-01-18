/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:48:11 by kycho             #+#    #+#             */
/*   Updated: 2021/01/18 23:21:06 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pwd_oldpwd_env(t_msh *msh)
{
	char		*real_pwd;
	t_dict		*pwd_dict;
	struct stat	sb1;
	struct stat	sb2;

	if (!(real_pwd = getcwd(NULL, 0)))
		exit_print_err(strerror(errno));
	if (stat(real_pwd, &sb1) == -1)
		exit_print_err(strerror(errno));
	pwd_dict = get_env_dict(msh->env, "PWD");
	if (pwd_dict == NULL ||
			(stat(pwd_dict->value, &sb2) == -1) ||
			(sb1.st_ino != sb2.st_ino))
	{
		set_env_dict(msh, "PWD", real_pwd);
	}
	free(real_pwd);
	set_env_dict(msh, "OLDPWD", NULL);
}

void	init_shlvl_env(t_msh *msh)
{
	t_dict	*dict;
	int		num;
	char	*value;

	if (!(dict = get_env_dict(msh->env, "SHLVL")) ||
			(is_numeric_long_str(dict->value) == FALSE))
	{
		set_env_dict(msh, "SHLVL", "1");
		return ;
	}
	if ((num = atoi(dict->value) + 1) <= 0)
		set_env_dict(msh, "SHLVL", "0");
	else
	{
		if (num > 1000)
		{
			value = ft_itoa(num);
			print_shlvl_err(msh->program_name, value);
			free(value);
			num = 1;
		}
		value = ft_itoa(num);
		set_env_dict(msh, "SHLVL", value);
		free(value);
	}
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
	init_pwd_oldpwd_env(msh);
	init_shlvl_env(msh);
}

/*
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
*/

void	init_msh_path(t_msh *msh)
{
	t_dict *env_dict;
	char *path;

	if ((env_dict = get_env_dict(msh->env, "PATH")) != NULL)
		path = env_dict->value;
	else
		path = DEFAULT_PATH;
	if (!(msh->path = ft_strdup(path)))
		exit_print_err(strerror(errno));
}

void	init_msh(char *program_name, t_msh *msh, char **env)
{
	int i;

	if (!(msh->launch_program_name = ft_strdup(program_name)))
		exit_print_err(strerror(errno));
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
	if (!(msh->pwd = ft_strdup(get_env_dict(msh->env, "PWD")->value)))
		exit_print_err(strerror(errno));
	msh->unset_pwd_flag = 0;
	if (!(msh->lstcmd = ft_strdup(program_name)))
		exit_print_err(strerror(errno));
}
