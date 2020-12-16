/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_msh_member.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:48:53 by kycho             #+#    #+#             */
/*   Updated: 2020/12/16 18:49:12 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_msh_member(t_msh *msh)
{
	t_list	*tmp_list;
	t_cmd	*tmp_cmd;

	while (msh->cmds)
	{
		free(msh->cmds->args);
		while (msh->cmds->redirection_files)
		{
			tmp_list = msh->cmds->redirection_files->next;
			free(msh->cmds->redirection_files);
			msh->cmds->redirection_files = tmp_list;
		}
		tmp_cmd = msh->cmds->next;
		free(msh->cmds);
		msh->cmds = tmp_cmd;
	}
	ft_lstclear(&(msh->tokens), free);
}
