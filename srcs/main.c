/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 08:58:59 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env)
{
	t_msh	msh;
	int		res;

	init_main(&msh, env);
	init_msh(av[0], &msh, env);
	res = ac;
	while (res)
	{
		show_prompt(&msh);
		if ((res = get_next_line(STDIN, &(msh.input))) == -1)
			exit_print_err("Error", "get_next_line fail!", EXIT_FAILURE);
		else
		{
			parsing(&msh, msh.input);
			t_cmd *cmd = msh.cmd;
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
				printf("length : %d\n", cmd->length);
				printf("type : %d\n", cmd->type);
				printf("pipes : %d %d\n", cmd->pipes[0], cmd->pipes[1]);
				printf("  --redirection start--\n");
				t_list *tmp = cmd->redirection_file;
				while(tmp)
				{
					printf("%s\n", (char *)tmp->content);
					tmp = tmp->next;
				}
				printf("  --redirection end--\n");
				cmd = cmd->next;
				printf("--------@@@@@@@@@@@@@-------------\n");
			}

			printf("1 msh.cmd : %p\n", msh.cmd);
			printf("1 msh.tokens : %p\n", msh.tokens);

			while(msh.cmd)
			{
				free(msh.cmd->args);
				while(msh.cmd->redirection_file)
				{
					t_list *tmp = msh.cmd->redirection_file->next;
					free(msh.cmd->redirection_file);
					msh.cmd->redirection_file = tmp;
				}
				t_cmd *tmp_cmd = msh.cmd->next;
				free(msh.cmd);
				msh.cmd = tmp_cmd;
			}

			printf("msh.cmd : %p\n", msh.cmd);
			ft_lstclear(&msh.tokens, free);
			printf("msh.tokens : %p\n", msh.tokens);
		}
		main_loop(&msh);
		free(msh.input);
	}
	return (0);
}
