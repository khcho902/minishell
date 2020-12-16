/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 18:27:10 by kycho            ###   ########.fr       */
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
	if (!(msh->env = (t_dict**)malloc(sizeof(t_dict*) * (msh->env_len + 1))))
		exit_print_err(strerror(errno));
	msh->env[msh->env_len] = NULL;
	i = 0;
	while (env[i])
	{
		key_len = ft_strchr(env[i], '=') - env[i];
		if (!(msh->env[i] = (t_dict*)malloc(sizeof(t_dict))))
			exit_print_err(strerror(errno));
		if (!(msh->env[i]->key = (char *)malloc(sizeof(char) * (key_len + 1))))
			exit_print_err(strerror(errno));
		ft_strlcat(msh->env[i]->key, env[i], key_len + 1);
		if (!(msh->env[i]->value = ft_strdup(env[i] + key_len + 1)))
			exit_print_err(strerror(errno));
		i++;
	}
}

void	init_msh_path(t_msh *msh)
{
	int		i;
	size_t	len;

	i = 0;
	while (msh->env[i])
	{
		len = ft_strlen("PATH");
		if (ft_strlen(msh->env[i]->key) > len)
			len = ft_strlen(msh->env[i]->key);
		if (ft_strncmp(msh->env[i]->key, "PATH", len) == 0)
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

void	free_msh_member(t_msh *msh)
{
	t_list	*tmp_list;
	t_cmd	*tmp_cmd;

	while (msh->cmds)
	{
		free(msh->cmds->args);
		while (msh->cmds->redirection_files)
		{
			tmp_list = msh->cmds->redirection_files->next;
			free(msh->cmds->redirection_files);
			msh->cmds->redirection_files = tmp_list;
		}
		tmp_cmd = msh->cmds->next;
		free(msh->cmds);
		msh->cmds = tmp_cmd;
	}
	ft_lstclear(&(msh->tokens), free);
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	if (argc == 0)
		return (1);
	init_msh(argv[0], &msh, env);
	res = 1;
	while (res)
	{
		ft_putstr_fd(msh.program_name, STDOUT);
		ft_putstr_fd("$ ", STDOUT);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("get_next_line fail!");
		else
		{
			if (parsing(&msh, input) == SUCCESS)
				printf("execute cmds!!!\n");
			else
				printf("parsing error : no execute cmds!!!\n");

			/*
			printf("------------------------------\n");
			printf("lstsize : %d\n", ft_lstsize(msh.tokens));
			printf("tokens : %p\n", msh.tokens);
			t_list *now = msh.tokens;
			while (now)
			{
				printf("|%s|\n", now->content);
				now = now->next;
			}
			printf("------------------------------\n");

			t_cmd *cmd = msh.cmds;
			while (cmd)
			{
				printf("--------@@@@@@@@@@@@@-------------\n");
				int i = 0;
				while(cmd->args[i])
				{
					printf("|%s|", cmd->args[i]);
					i++;
				}
				printf("\n");
				printf("lenght : %d\n", cmd->length);
				printf("type : %d\n", cmd->type);
				printf("pipes : %d %d\n", cmd->pipes[0], cmd->pipes[1]);
				printf("  --redirection start--\n");
				t_list *tmp = cmd->redirection_files;
				while(tmp)
				{
					printf("%s\n", tmp->content);
					tmp = tmp->next;
				}
				printf("  --redirection end--\n");
				cmd = cmd->next;
				printf("--------@@@@@@@@@@@@@-------------\n");
			}
			printf("1 msh.cmds : %p\n", msh.cmds);
			printf("1 msh.tokens : %p\n", msh.tokens);
			*/

			free_msh_member(&msh);
			free(input);
			/*
			printf("msh.cmds : %p\n", msh.cmds);
			printf("msh.tokens : %p\n", msh.tokens);
			*/
		}
	}
	return (0);
}
