/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 19:37:19 by kycho             #+#    #+#             */
/*   Updated: 2020/12/20 19:39:28 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sanitize_env_sub(char **res_str, char *og_str, t_msh *msh, char *tmp)
{
	char *tmp2;

	if (og_str[1] == '$' || ('1' <= og_str[1] && og_str[1] <= '9'))
		return (2);
	else if (og_str[1] == '\0')
	{
		append_char_to_str(res_str, '$');
		return (1);
	}
	else if (og_str[1] == '0')
	{
		if (!(tmp = ft_strjoin("-", msh->program_name)))
			exit_print_err(strerror(errno));
		if (!(tmp2 = ft_strjoin(*res_str, tmp)))
			exit_print_err(strerror(errno));
		free(tmp);
		free(*res_str);
		*res_str = tmp2;
		return (2);
	}
	else if (og_str[1] == '?')
	{
		if (!(tmp = ft_strjoin(*res_str, ft_itoa(msh->exit_status))))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
		return (2);
	}
	return (-1);
}

int		sanitize_env(char **res_str, char *og_str, t_msh *msh)
{
	int		env_len;
	char	*env_key;
	t_dict	*env_dict;
	char	*tmp;

	if ((env_len = sanitize_env_sub(res_str, og_str, msh, NULL)) != -1)
		return (env_len);
	env_len = 1;
	while (og_str[env_len] != '\0' && (og_str[env_len] == '_'
				|| ('0' <= og_str[env_len] && og_str[env_len] <= '9')
				|| ('a' <= og_str[env_len] && og_str[env_len] <= 'z')
				|| ('A' <= og_str[env_len] && og_str[env_len] <= 'Z')))
		env_len++;
	if (!(env_key = (char *)malloc(sizeof(char) * (env_len + 1))))
		exit_print_err(strerror(errno));
	ft_strlcpy(env_key, og_str + 1, env_len);
	env_dict = get_env_dict(msh->env, env_key);
	free(env_key);
	if (env_dict == NULL)
		return (env_len);
	if (!(tmp = ft_strjoin(*res_str, env_dict->value)))
		exit_print_err(strerror(errno));
	free(*res_str);
	*res_str = tmp;
	return (env_len);
}

int		sanitize_backslash(char **res_str, char *og_str, int in_dquotes)
{
	if (in_dquotes && og_str[1] != '"' && og_str[1] != '\\')
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

void	sanitize_token(t_list *token, t_msh *msh)
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
		else if (og_str[i] == '$')
			i += sanitize_env(&res_str, og_str + i, msh);
		else
			append_char_to_str(&res_str, og_str[i++]);
	}
	free(token->content);
	token->content = res_str;
}
