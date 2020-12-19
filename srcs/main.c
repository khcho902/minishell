/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/19 14:55:36 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*compare_arg(t_msh *msh)
{
	if (!ft_strcmp(msh->cmds->args[0], "exit"))
		return (&do_exit);
	else if (!ft_strcmp(msh->cmds->args[0], "echo"))
		return (&do_echo);
	else if (!ft_strcmp(msh->cmds->args[0], "env"))
		return (&do_env);
	else if (!ft_strcmp(msh->cmds->args[0], "export"))
		return (&do_export);
	else if (!ft_strcmp(msh->cmds->args[0], "pwd"))
		return (&do_pwd);
	else if (!ft_strcmp(msh->cmds->args[0], "unset"))
		return (&do_unset);
	else if (!ft_strcmp(msh->cmds->args[0], "cd"))
		return (&do_cd);
	else
		return (&executor);
}

int		main_loop(t_msh *msh)
{
	int			ret;
	int			fd_in;
	int			fd_out;
	t_exe_fn	func;

	ret = EXIT_SUCCESS;
	while (msh->cmds)
	{
		func = compare_arg(msh);

		t_list		*l;

		l = msh->cmds->redirection_files;
		fd_in = -1;
		fd_out = -1;
		while (l)
		{
			if (!ft_strcmp("<", l->content))
			{
				if (fd_in != -1)
					close(fd_in);
				l = l->next;
				if (l && (fd_in = open(l->content, O_RDONLY,
							S_IRUSR | S_IWUSR | S_IRWXG | S_IROTH)) == -1)
						exit_print_err(strerror(errno));
			}
			else if (!ft_strcmp(">", l->content))
			{
				if (fd_out != -1)
					close(fd_out);
				l = l->next;
				if (l && (fd_out = open(l->content,
							O_WRONLY | O_TRUNC | O_CREAT,
							S_IRUSR | S_IWUSR | S_IRWXG | S_IROTH)) == -1)
						exit_print_err(strerror(errno));
			}
			else if (!ft_strcmp(">>", l->content))
			{
				if (fd_out != -1)
					close(fd_out);
				l = l->next;
				if (l && (fd_out = open(l->content,
							O_WRONLY | O_APPEND | O_CREAT,
							S_IRUSR | S_IWUSR | S_IRWXG | S_IROTH)) == -1)
						exit_print_err(strerror(errno));
			}
			l = l->next;
		}
		if (func != &executor && msh->cmds->type == TYPE_DEFAULT &&
				fd_in == -1 && fd_out == -1 && (msh->cmds->prev == NULL ||
				(msh->cmds->prev && msh->cmds->prev->type == TYPE_DEFAULT)))
			ret = func(msh);
		else
		{
			pid_t	pid;
			int		pipe_open;
			int		status;

			pipe_open = 0;
			if (msh->cmds->type == TYPE_PIPE)
			{
				pipe_open = 1;
				if (pipe(msh->cmds->pipes) == -1)
					exit_print_err(strerror(errno));
			}
			if ((pid = fork()) == -1)
				exit_print_err(strerror(errno));
			if (pid == 0)
			{
				if (msh->cmds->type == TYPE_PIPE &&
						dup2(msh->cmds->pipes[PIPE_IN], STDOUT) < 0)
					exit_print_err(strerror(errno));
				if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE &&
						dup2(msh->cmds->prev->pipes[PIPE_OUT], STDIN) < 0)
					exit_print_err(strerror(errno));
				if (fd_in != -1 && dup2(fd_in, STDIN) < 0)
					exit_print_err(strerror(errno));
				if (fd_out != -1 && dup2(fd_out, STDOUT) < 0)
					exit_print_err(strerror(errno));
				ret = func(msh);
				exit(ret);
			}
			else
			{
				waitpid(pid, &status, 0);
				if (pipe_open)
				{
					close(msh->cmds->pipes[PIPE_IN]);
					if (!msh->cmds->next || msh->cmds->type == TYPE_DEFAULT)
						close(msh->cmds->pipes[PIPE_OUT]);
				}
				if (msh->cmds->prev && msh->cmds->prev->type == TYPE_PIPE)
					close(msh->cmds->prev->pipes[PIPE_OUT]);
				if (WIFEXITED(status))
					ret = WEXITSTATUS(status);
			}
		}
		msh->cmds = msh->cmds->next;
	}
	return (ret);
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	int		ret;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	res = argc;
	ret = EXIT_SUCCESS;
	while (res)
	{
		show_prompt(&msh);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("get_next_line fail!");
		else
		{
			if (parsing(&msh, input) == SUCCESS)
				ret = main_loop(&msh);
			free_msh_member(&msh);
			free(input);
		}
	}
	return (ret);
}
