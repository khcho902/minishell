/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_valid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:32:52 by kycho             #+#    #+#             */
/*   Updated: 2021/01/09 20:03:40 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			check_token_valid(char *program_name, t_list *now)
{
	int		before_type;

	before_type = TOKEN_NONE;
	while (now)
	{
		if (((char*)now->content)[0] == '|')
		{
			if (before_type != TOKEN_WORD)
				return (print_syntax_err(program_name, now->content, FALSE));
			before_type = TOKEN_PIPE;
		}
		else if (((char *)now->content)[0] == ';')
		{
			if (before_type != TOKEN_WORD)
				return (print_syntax_err(program_name, now->content, FALSE));
			before_type = TOKEN_SEMICOLON;
		}
		else if (((char *)now->content)[0] == '>' ||
				((char *)now->content)[0] == '<')
		{
			if (before_type == TOKEN_REDIRECTION)
				return (print_syntax_err(program_name, now->content, FALSE));
			before_type = TOKEN_REDIRECTION;
		}
		else
			before_type = TOKEN_WORD;
		now = now->next;
	}
	if (before_type == TOKEN_REDIRECTION)
		return (print_syntax_err(program_name, "newline", FALSE));
	if (before_type == TOKEN_PIPE)
		return (ERROR);
	return (SUCCESS);
}
