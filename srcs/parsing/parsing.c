/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:23:23 by kycho             #+#    #+#             */
/*   Updated: 2020/12/16 18:35:22 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		parsing(t_msh *msh, char *input)
{
	if (msh == NULL || input == NULL)
		return (ERROR);
	split_token(input, &(msh->tokens), 0);
	if (check_token_valid(msh->program_name, msh->tokens) == ERROR)
		return (ERROR);
	making_cmd(msh);
	return (SUCCESS);
}
