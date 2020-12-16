/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 18:50:59 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
