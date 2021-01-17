/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_replaced_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 18:52:46 by kycho             #+#    #+#             */
/*   Updated: 2021/01/17 18:52:56 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		pass_backslash(char **res_str, char *og_str)
{
	append_char_to_str(res_str, '\\');
	if (og_str[1])
	{
		append_char_to_str(res_str, og_str[1]);
		return (2);
	}
	return (1);
}

int		pass_single_quotes(char **res_str, char *og_str, int in_dquotes)
{
	int i;

	if (in_dquotes)
	{
		append_char_to_str(res_str, '\'');
		return (1);
	}
	i = 1;
	append_char_to_str(res_str, '\'');
	while (og_str[i] && og_str[i] != '\'')
	{
		append_char_to_str(res_str, og_str[i]);
		i++;
	}
	if (og_str[i] == '\'')
	{
		append_char_to_str(res_str, '\'');
		i++;
	}
	return (i);
}

char	*get_env_replaced_input(t_msh *msh, char *input)
{
	char	*res_str;
	int		i;
	int		in_dquotes;

	in_dquotes = FALSE;
	if (!(res_str = ft_strdup("")))
		exit_print_err(strerror(errno));
	i = 0;
	while (input[i])
	{
		if (input[i] == '\\')
			i += pass_backslash(&res_str, input + i);
		else if (input[i] == '\'')
			i += pass_single_quotes(&res_str, input + i, in_dquotes);
		else if (input[i] == '"' && ++i)
		{
			append_char_to_str(&res_str, '"');
			in_dquotes ^= TRUE;
		}
		else if (input[i] == '$')
			i += sanitize_env(&res_str, input + i, msh, in_dquotes);
		else
			append_char_to_str(&res_str, input[i++]);
	}
	return (res_str);
}
