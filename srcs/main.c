/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/24 14:25:47 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int		check_quotes_valid(char *program_name, char *input)
{
	int in_dquotes;
	int i;

	in_dquotes = FALSE;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\\')
			i++;
		else if (in_dquotes == FALSE && input[i] == '\'')
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

int		check_input_valid(t_msh *msh, char *input)
{
	t_list	*tokens;

	if (check_quotes_valid(msh->program_name, input) == ERROR)
	{
		msh->exit_status = 258;
		return (ERROR);
	}
	split_token(input, &tokens, METACHARACTER);
	if (check_token_valid(msh->program_name, tokens) == ERROR)
	{
		msh->exit_status = 258;
		ft_lstclear(&(tokens), free);
		return (ERROR);
	}
	ft_lstclear(&(tokens), free);
	return (SUCCESS);
}

void	main_loop(t_msh *msh, char *input)
{
	t_list	*jobs;
	t_list	*job_now;

	split_token(input, &jobs, ";");
	job_now = jobs;
	while (job_now)
	{
		if (ft_strcmp(job_now->content, ";") != 0)
		{
			if (parsing(msh, (char *)job_now->content) == SUCCESS)
				executing(msh);
			free_msh_member(msh);
		}
		job_now = job_now->next;
	}
	ft_lstclear(&(jobs), free);
}

int		get_command_line(char **input)
{
	int		res;
	char	*input2;
	char	*tmp;

	*input = ft_strdup("");
	while (TRUE)
	{
		if ((res = get_next_line(STDIN, &input2)) == -1)
			exit_print_err("get_next_line fail");
		tmp = ft_strjoin(*input, input2);
		free(input2); // 추가 
		free(*input);
		*input = tmp;
		if (res == 0)
		{
			ft_putstr_fd("  \b\b", STDOUT);
			//if ((ft_strlen(input2) != 0 || ft_strlen(*input)))
			if (ft_strlen(*input) != 0)
				continue;
			ft_putstr_fd("exit\n", STDOUT);
		}
		break ;
	}
	return (res);
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	init_signal();
	if (argc >= 2 && ft_strcmp(argv[1], "-c") == 0)
	{
		if (!(input = argv[2]))
		{
			print_execute_err(argv[0], "-c", "option requires an argument");
			exit(2);
		}
		if (check_input_valid(&msh, input) != ERROR)
			main_loop(&msh, input);
		exit(msh.exit_status & 255);
	}
	show_logo();
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		res = get_command_line(&input);
		if (check_input_valid(&msh, input) != ERROR)
			main_loop(&msh, input);
		free(input);
	}
	return (msh.exit_status & 255);
}
