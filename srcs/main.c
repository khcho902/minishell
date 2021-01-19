/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/20 01:08:50 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_input_valid(char *program_name, char *input)
{
	int in_dquotes;
	int i;

	in_dquotes = FALSE;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\\')
			i++;
		else if (in_dquotes == FALSE && input[i] == '\'' )
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
			if (input[i] != '\'')
				return (print_syntax_err(program_name, "'", TRUE));
		}
		else if (input[i] == '"')
			in_dquotes ^= TRUE;
		i++;
	}
	if (in_dquotes == TRUE)
		return (print_syntax_err(program_name, "\"", TRUE));
	return (SUCCESS);
}

int		main(int argc, char **argv, char **env)
{
	/*
	char **res2 = ft_split("/bin:::::::", ':');
	int i = 0;
	while (res2[i])
	{
		dprintf(2, "|%s|\n", res2[i]);
		i++;
	}
	*/
/*
	int i = 0;
	while (env[i])
	{
		dprintf(2, "|%s|\n", env[i]);
		i++;
	}
*/


	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	init_signal();

	/****************************************************/
	if (argc == 3 && ft_strcmp("-c", argv[1]) == 0)
	{
		input = argv[2];
		/*--------------------------------------------------------------------*/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			return (msh.exit_status & 255);
		}


		t_list *tokens = NULL;
		split_token(input, &tokens, METACHARACTER, 0);
		if (check_token_valid(msh.program_name, tokens) == ERROR)
		{
			msh.exit_status = 258;
			ft_lstclear(&(tokens), free);
			return (msh.exit_status & 255);
		}
		ft_lstclear(&(tokens), free);



		t_list *jobs = NULL;
		t_list *job_now = NULL;
		split_token(input, &jobs, ";", 0);
		job_now = jobs;
		while (job_now)
		{
			if (ft_strcmp(job_now->content, ";") == 0)
			{
				job_now = job_now->next;
				continue;
			}
			if (parsing(&msh, (char *)job_now->content) == SUCCESS)
				executing(&msh);
			free_msh_member(&msh);

			job_now = job_now->next;
		}
		ft_lstclear(&(jobs), free);

		return (msh.exit_status & 255);
	}
	/****************************************************/

	show_logo();
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		if ((res = get_next_line(STDIN, &input)) == -1)
			exit_print_err("get_next_line fail");
		/*--------------------------------------------------------------------*/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			continue;
		}


		t_list *tokens = NULL;
		split_token(input, &tokens, METACHARACTER, 0);
		if (check_token_valid(msh.program_name, tokens) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			ft_lstclear(&(tokens), free);
			continue;
		}
		ft_lstclear(&(tokens), free);


		t_list *jobs = NULL;
		t_list *job_now = NULL;
		split_token(input, &jobs, ";", 0);
		job_now = jobs;
		while (job_now)
		{
			if (ft_strcmp(job_now->content, ";") == 0)
			{
				job_now = job_now->next;
				continue;
			}
			if (parsing(&msh, (char *)job_now->content) == SUCCESS)
			{
				executing(&msh);
			}
			free_msh_member(&msh);

			job_now = job_now->next;
		}
		ft_lstclear(&(jobs), free);


		free(input);
	}
	return (msh.exit_status & 255);
}
