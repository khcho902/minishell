/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 19:37:19 by kycho             #+#    #+#             */
/*   Updated: 2021/01/09 16:29:39 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sanitize_backslash(char **res_str, char *og_str, int in_dquotes)
{
	if (in_dquotes && og_str[1] != '"' && og_str[1] != '\\' && og_str[1] != '$')
		append_char_to_str(res_str, '\\');
	if (og_str[1])
	{
		append_char_to_str(res_str, og_str[1]);
		return (2);
	}
	return (1);
}

int		sanitize_single_quotes(char **res_str, char *og_str, int in_dquotes)
{
	int i;

	if (in_dquotes)
	{
		append_char_to_str(res_str, '\'');
		return (1);
	}
	i = 1;
	while (og_str[i] && og_str[i] != '\'')
	{
		append_char_to_str(res_str, og_str[i]);
		i++;
	}
	if (og_str[i] == '\'')
		i++;
	return (i);
}

void	sanitize_token(t_list *token)
{
	char	*og_str;
	char	*res_str;
	int		i;
	int		in_dquotes;

	in_dquotes = FALSE;
	og_str = token->content;
	if (!(res_str = ft_strdup("")))
		exit_print_err(strerror(errno));
	i = 0;
	while (og_str[i])
	{
		if (og_str[i] == '\\')
			i += sanitize_backslash(&res_str, og_str + i, in_dquotes);
		else if (og_str[i] == '\'')
			i += sanitize_single_quotes(&res_str, og_str + i, in_dquotes);
		else if (og_str[i] == '"' && ++i)
			in_dquotes ^= TRUE;
		else
			append_char_to_str(&res_str, og_str[i++]);
	}
	free(token->content);
	token->content = res_str;
}
