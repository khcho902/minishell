/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_valid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:32:52 by kycho             #+#    #+#             */
/*   Updated: 2020/12/16 18:33:34 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_token_valid(char *program_name, t_list *now)
{
	int		before_type;

	before_type = 0;
	while (now)
	{
		if (((char*)now->content)[0] == '|' || ((char *)now->content)[0] == ';')
		{
			if (before_type != 1)
				return (print_syntax_err(program_name, now->content, FALSE));
			before_type = 2;
		}
		else if (((char *)now->content)[0] == '>' ||
				((char *)now->content)[0] == '<')
		{
			if (before_type == 3)
				return (print_syntax_err(program_name, now->content, FALSE));
			before_type = 3;
		}
		else
			before_type = 1;
		now = now->next;
	}
	if (before_type == 3)
		return (print_syntax_err(program_name, "newline", FALSE));
	return (SUCCESS);
}
