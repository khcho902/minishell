/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 18:40:41 by kycho             #+#    #+#             */
/*   Updated: 2021/01/18 01:53:39 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exception_handler_sub(
		char **res_str, char *og_str, t_msh *msh, int in_dquotes)
{
	char *tmp;

	if (og_str[1] == '"' || og_str[1] == '\'')
	{
		if (og_str[1] == '"' && in_dquotes == TRUE)
			append_char_to_str(res_str, '$');
		return (1);
	}
	if (og_str[1] == '0')
	{
		if (!(tmp = ft_strjoin(*res_str, msh->launch_program_name)))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(tmp = ft_strjoin(*res_str, ft_itoa(msh->exit_status))))
			exit_print_err(strerror(errno));
	}
	free(*res_str);
	*res_str = tmp;
	return (2);
}

int		exception_handler(
		char **res_str, char *og_str, t_msh *msh, int in_dquotes)
{
	if (og_str[1] == '$' || ('1' <= og_str[1] && og_str[1] <= '9'))
		return (2);
	else if (is_in_charset(og_str[1], "\"'0?") == TRUE)
	{
		return (exception_handler_sub(res_str, og_str, msh, in_dquotes));
	}
	else if (og_str[1] == '\0' || !(og_str[1] == '_' || ft_isalnum(og_str[1])))
	{
		append_char_to_str(res_str, '$');
		return (1);
	}
	return (-1);
}

void	sanitize_env_sub(char **res_str, int in_dquotes, t_dict *env_dict)
{
	char	*tmp;
	char	*tmp2;

	if (in_dquotes)
	{
		if (!(tmp2 = insert_char_before_set(env_dict->value, "\"\\", '\\')))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(tmp2 = insert_char_before_set(env_dict->value, "'\"\\", '\\')))
			exit_print_err(strerror(errno));
	}
	if (!(tmp = ft_strjoin(*res_str, tmp2)))
		exit_print_err(strerror(errno));
	free(tmp2);
	free(*res_str);
	*res_str = tmp;
}

int		sanitize_env(char **res_str, char *og_str, t_msh *msh, int in_dquotes)
{
	int		env_len;
	char	*env_key;
	t_dict	*env_dict;

	if ((env_len = exception_handler(res_str, og_str, msh, in_dquotes)) != -1)
		return (env_len);
	env_len = 1;
	while (og_str[env_len] != '\0' &&
			(og_str[env_len] == '_' || ft_isalnum(og_str[env_len])))
		env_len++;
	if (!(env_key = (char *)malloc(sizeof(char) * (env_len + 1))))
		exit_print_err(strerror(errno));
	ft_strlcpy(env_key, og_str + 1, env_len);
	env_dict = get_env_dict(msh->env, env_key);
	free(env_key);
	if (env_dict == NULL || env_dict->value == NULL)
		return (env_len);
	sanitize_env_sub(res_str, in_dquotes, env_dict);
	return (env_len);
}
