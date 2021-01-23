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

int		set_redirection_fd(t_msh *msh, t_cmd *cmd)
{
	t_list *list;

	list = cmd->redirection_files;
	while(list)
	{
		if (!ft_strcmp(">", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd = open(list->content, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		else if (!ft_strcmp(">>", list->content))
		{
			if (cmd->output_fd != -1)
				close(cmd->output_fd);
			list = list->next;
			if ((cmd->output_fd = open(list->content, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		else if (!ft_strcmp("<", list->content))
		{
			if (cmd->input_fd != -1)
				close(cmd->input_fd);
			list = list->next;
			if ((cmd->input_fd = open(list->content, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
				return (print_execute_err(msh->program_name, list->content, strerror(errno)));
		}
		list = list->next;
	}
	return (SUCCESS);
}
