/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/04 12:31:45 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

		if (parsing(&msh, input) == SUCCESS)
		{
			executing(&msh);
		}

		free_msh_member(&msh);
		return (msh.exit_status & 255);
	}
	/****************************************************/

	show_logo();
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("get_next_line fail");
		else
		{
			if (parsing(&msh, input) == SUCCESS)
				executing(&msh);
			free_msh_member(&msh);
			free(input);
		}
	}
	return (EXIT_SUCCESS);
}
