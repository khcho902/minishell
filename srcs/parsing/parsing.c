/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:23:23 by kycho             #+#    #+#             */
/*   Updated: 2021/01/05 17:05:52 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		parsing(t_msh *msh, char *input)
{
	if (msh == NULL || input == NULL)
		return (ERROR);

	split_token(input, &(msh->tokens), 0);
	if (check_token_valid(msh->program_name, msh->tokens) == ERROR)
	{
		msh->exit_status = 258;
		return (ERROR);
	}
	making_cmd(msh);
	return (SUCCESS);
}
