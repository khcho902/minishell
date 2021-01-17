/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:23:23 by kycho             #+#    #+#             */
/*   Updated: 2021/01/13 21:09:28 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sanitize_env_sub2(char **res_str, char *og_str, t_msh *msh, char *tmp)
{
	if (og_str[1] == '$' || ('1' <= og_str[1] && og_str[1] <= '9'))
		return (2);
	else if (og_str[1] == '"' || og_str[1] == '\'')
		return (1);
	//else if (og_str[1] == '\0' || og_str[1] == '\\')
	else if (og_str[1] == '0')
	{
		if (!(tmp = ft_strjoin(*res_str, msh->program_name)))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
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
	else if (og_str[1] == '_')
	{
		if (!(tmp = ft_strjoin(*res_str, ft_itoa(msh->exit_status))))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
		return (2);
	}
	else if (!(og_str[1] != '\0' &&
				(('0' <= og_str[1] && og_str[1] <= '9')
				|| ('a' <= og_str[1] && og_str[1] <= 'z')
				|| ('A' <= og_str[1] && og_str[1] <= 'Z'))))
	{
		append_char_to_str(res_str, '$');
		return (1);
	}
	return (-1);
}

int		sanitize_env2(char **res_str, char *og_str, t_msh *msh, int in_dquotes)
{
	int		env_len;
	char	*env_key;
	t_dict	*env_dict;
	char	*tmp;
	char	*tmp2;   // 추가함 

	if ((env_len = sanitize_env_sub2(res_str, og_str, msh, NULL)) != -1)
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

	/****/
	/*
	if (ft_strcmp("PWD", env_key) == 0)
	{
		if (!(tmp = ft_strjoin(*res_str, msh->pwd)))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
		return (env_len);
	}
	if (ft_strcmp("OLDPWD", env_key) == 0)
	{
		if (!(tmp = ft_strjoin(*res_str, msh->oldpwd)))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;	
		return (env_len);
	}
	*/
	/****/

	env_dict = get_env_dict(msh->env, env_key);
	free(env_key);
	if (env_dict == NULL || env_dict->value == NULL)
		return (env_len);
	if (in_dquotes)
	{
		if (!(tmp2 = insert_char_before_set(env_dict->value, "\"\\", '\\')))
			exit_print_err(strerror(errno));
	}
	else
	{
		if (!(tmp2 = insert_char_before_set(env_dict->value, "'\"\\", '\\')))  /// 추가함
			exit_print_err(strerror(errno));               // 추가함
	}
	//if (!(tmp = ft_strjoin(*res_str, env_dict->value)))
	if (!(tmp = ft_strjoin(*res_str, tmp2)))          // 위에꺼에서 변경
		exit_print_err(strerror(errno));
	free(tmp2);                                       ///  추가함
	free(*res_str);
	*res_str = tmp;
	return (env_len);
}

int		sanitize_backslash2(char **res_str, char *og_str)
{
	/*
	if (in_dquotes && og_str[1] != '"' && og_str[1] != '\\')
		append_char_to_str(res_str, '\\');
	*/
	append_char_to_str(res_str, '\\');
	if (og_str[1])
	{
		append_char_to_str(res_str, og_str[1]);
		return (2);
	}
	return (1);
}

int		sanitize_single_quotes2(char **res_str, char *og_str, int in_dquotes)
{
	int i;

	if (in_dquotes)
	{
		append_char_to_str(res_str, '\'');
		return (1);
	}
	i = 1;
	append_char_to_str(res_str, '\''); // 추가함
	while (og_str[i] && og_str[i] != '\'')
	{
		append_char_to_str(res_str, og_str[i]);
		i++;
	}
	if (og_str[i] == '\'')
	{
		append_char_to_str(res_str, '\'');  // 추가함
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

//	dprintf(2, "|%s|\n", *input);

	while (input[i])
	{
		if (input[i] == '\\')
			i += sanitize_backslash2(&res_str, input + i);
		else if (input[i] == '\'')
			i += sanitize_single_quotes2(&res_str, input + i, in_dquotes);
		else if (input[i] == '"' && ++i)
		{
			append_char_to_str(&res_str, '"');
			in_dquotes ^= TRUE;
		}
		else if (input[i] == '$')
			i += sanitize_env2(&res_str, input + i, msh, in_dquotes);
		else
			append_char_to_str(&res_str, input[i++]);
//		dprintf(2, "????? %d\n", i);
	}
//	free((*input));
//	(*input) = res_str;
	return (res_str);
}



int		parsing(t_msh *msh, char *input)
{
	if (msh == NULL || input == NULL)
		return (ERROR);

	char *env_replaced_input;
	
	/*******/
	env_replaced_input = get_env_replaced_input(msh, input);
	/*******/

//	dprintf(2, "env_replaced_input : [%s]\n", env_replaced_input); 

	split_token(env_replaced_input, &(msh->tokens), METACHARACTER, 0);
/*
	t_list *l = msh->tokens;
	while (l)
	{
		dprintf(2, "|%s|\n", l->content);
		l = l->next;
	}
*/
	if (check_token_valid(msh->program_name, msh->tokens) == ERROR)
	{
		msh->exit_status = 258;
		return (ERROR);
	}
	making_cmd(msh);
	free(env_replaced_input);
	return (SUCCESS);
}
