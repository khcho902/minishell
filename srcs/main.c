/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/05 14:01:03 by kycho            ###   ########.fr       */
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
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	init_signal();

	/****************************************************/
	if (argc == 3 && ft_strcmp("-c", argv[1]) == 0)
	{
		input = argv[2];
		/**********************/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			return (msh.exit_status & 255);
		}
		split_token(input, &(msh.tokens), 0);
		if (check_token_valid(msh.program_name, msh.tokens) == ERROR)
		{
			msh.exit_status = 258;
			return (msh.exit_status & 255);
		}

		if (parsing(&msh, input) == SUCCESS)
			executing(&msh);

		free_msh_member(&msh);
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
		/**********************/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			continue;
		}
		split_token(input, &(msh.tokens), 0);
		if (check_token_valid(msh.program_name, msh.tokens) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			continue;
		}



		if (parsing(&msh, input) == SUCCESS)
			executing(&msh);

		free_msh_member(&msh);
		free(input);
	}
	return (EXIT_SUCCESS);
}
