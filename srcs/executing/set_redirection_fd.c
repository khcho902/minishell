/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirection_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 06:00:22 by kycho             #+#    #+#             */
/*   Updated: 2021/01/21 06:00:52 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		set_output_redirection_fd(t_msh *msh, t_cmd *cmd, t_list **list)
{
	int		flag;
	mode_t	mode;

	if (!ft_strcmp(">", (*list)->content))
		flag = O_WRONLY | O_TRUNC | O_CREAT;
	else
		flag = O_WRONLY | O_APPEND | O_CREAT;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
	*list = (*list)->next;
	if ((cmd->output_fd = open((*list)->content, flag, mode)) == -1)
	{
		return (print_execute_err(
			msh->program_name, (*list)->content, strerror(errno)));
	}
	return (SUCCESS);
}

int		set_input_redirection_fd(t_msh *msh, t_cmd *cmd, t_list **list)
{
	int		flag;
	mode_t	mode;

	flag = O_RDONLY;
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	*list = (*list)->next;
	if ((cmd->input_fd = open((*list)->content, flag, mode)) == -1)
	{
		return (print_execute_err(
			msh->program_name, (*list)->content, strerror(errno)));
	}
	return (SUCCESS);
}

int		set_redirection_fd(t_msh *msh, t_cmd *cmd)
{
	t_list *list;

	list = cmd->redirection_files;
	while (list)
	{
		if (!ft_strcmp(">", list->content) ||
			!ft_strcmp(">>", list->content))
		{
			if (set_output_redirection_fd(msh, cmd, &list) == ERROR)
				return (ERROR);
		}
		else if (!ft_strcmp("<", list->content))
		{
			if (set_input_redirection_fd(msh, cmd, &list) == ERROR)
				return (ERROR);
		}
		list = list->next;
	}
	return (SUCCESS);
}
