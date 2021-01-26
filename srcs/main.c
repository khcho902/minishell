/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/25 00:23:18 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

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
		free(input2);
		free(*input);
		*input = tmp;
		if (res == 0)
		{
			ft_putstr_fd("  \b\b", STDOUT);
			if (ft_strlen(*input) != 0)
				continue;
			ft_putstr_fd("exit\n", STDOUT);
		}
		break ;
	}
	return (res);
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

void	onetime_run(char **argv, t_msh *msh)
{
	char *input;

	msh->c_option_flag = TRUE;
	if (!(input = argv[2]))
	{
		print_execute_err(argv[0], "-c", "option requires an argument");
		exit(2);
	}
	if (check_input_valid(msh, input) != ERROR)
		main_loop(msh, input);
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
		onetime_run(argv, &msh);
		exit(g_exit_status & 255);
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
	return (g_exit_status & 255);
}
