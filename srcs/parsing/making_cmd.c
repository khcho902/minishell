/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   making_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kycho <kycho@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 18:34:02 by kycho             #+#    #+#             */
/*   Updated: 2020/12/19 23:08:54 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char_to_str(char **str, char c)
{
	char	*tmp;
	size_t	str_len;

	str_len = ft_strlen(*str);
	if (!(tmp = malloc(sizeof(char) * (str_len + 2))))
		exit_print_err(strerror(errno));
	ft_strlcpy(tmp, *str, str_len + 1);
	tmp[str_len] = c;
	tmp[str_len + 1] = '\0';
	free(*str);
	*str = tmp;
}


int		sanitize_token_env(char **res_str, char *og_str, t_msh *msh)
{
	int env_len;
	char *env_key;
	t_dict *env_dict;
	char *tmp;

	if (og_str[1] == '$')
		return (2);
	if (og_str[1] == '0')
	{
		char *tmp2 = ft_strjoin("-", msh->program_name);
	
		if (!(tmp = ft_strjoin(*res_str, tmp2)))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
		return (2);
	}
	if (og_str[1] == '?')
	{
		if (!(tmp = ft_itoa(msh->exit_status)))
			exit_print_err(strerror(errno));
		char *tmp2 = ft_strjoin(*res_str, tmp);

		free(*res_str);
		*res_str = tmp2;
		return (2);
	}

	if (og_str[1] == '\0')
		return (1);

	env_len = 1;
	while (og_str[env_len] != '\0' && !is_in_charset(og_str[env_len], " '\"$\\"))
			env_len++;
	if (!(env_key = (char *)malloc(sizeof(char) * (env_len + 1))))
		exit_print_err(strerror(errno));
	ft_strlcpy(env_key, og_str + 1, env_len);
	env_dict = get_env_dict(msh->env, env_key);
	free(env_key);
	if (env_dict != NULL)
	{
		if (!(tmp = ft_strjoin(*res_str, env_dict->value)))
			exit_print_err(strerror(errno));
		free(*res_str);
		*res_str = tmp;
	}
	return (env_len);
}


void	sanitize_token(t_list *token, t_msh *msh)
{
	char	*og_str;
	char	*res_str;
	int		i;

	og_str = token->content;
	res_str = malloc(sizeof(char));
	res_str[0] = '\0';
	i = 0;
	while (og_str[i])
	{
		if (og_str[i] == '\\')
		{
			i++;
			if (og_str[i])
			{
				append_char_to_str(&res_str, og_str[i]);
				i++;
			}
		}
		else if (og_str[i] == '\'')
		{
			i++;
			while(og_str[i] && og_str[i] != '\'')
			{
				append_char_to_str(&res_str, og_str[i]);
				i++;
			}
			if (og_str[i] == '\'')
				i++;
		}
		else if (og_str[i] == '"')
		{
			i++;
		}
		else if (og_str[i] == '$')
		{
			i += sanitize_token_env(&res_str, og_str + i, msh);
		}
		else
		{
			append_char_to_str(&res_str, og_str[i]);
			i++;
		}
	}
	free(token->content);
	token->content = res_str;
}


//----------------------------------------------------------------------------

t_cmd	*get_new_cmd(t_cmd *previous)
{
	t_cmd	*new_cmd;

	if (!(new_cmd = (t_cmd*)malloc(sizeof(t_cmd))))
		exit_print_err(strerror(errno));
	if (!(new_cmd->args = (char**)malloc(sizeof(char*))))
		exit_print_err(strerror(errno));
	new_cmd->args[0] = NULL;
	new_cmd->length = 0;
	new_cmd->type = TYPE_DEFAULT;
	new_cmd->pipes[0] = -1;
	new_cmd->pipes[1] = -1;
	new_cmd->redirection_files = NULL;
	new_cmd->previous = NULL;
	new_cmd->next = NULL;
	if (previous != NULL)
	{
		previous->next = new_cmd;
		new_cmd->previous = previous;
	}
	return (new_cmd);
}

void	add_args(t_cmd *cmd, t_list *token, t_msh *msh)
{
	int		i;
	char	**tmp;

	if (!(tmp = (char**)malloc(sizeof(char*) * (cmd->length + 2))))
		exit_print_err(strerror(errno));
	tmp[cmd->length + 1] = NULL;
	i = 0;
	while (i < cmd->length)
	{
		tmp[i] = cmd->args[i];
		i++;
	}
	sanitize_token(token, msh);
	tmp[i] = token->content;
	free(cmd->args);
	cmd->args = tmp;
	cmd->length++;
}

void	add_redirection_file(t_cmd *cmd, t_list **token, t_msh *msh)
{
	t_list *lstnew;

	sanitize_token(*token, msh);
	if (!(lstnew = ft_lstnew((*token)->content)))
		exit_print_err(strerror(errno));
	if (cmd->redirection_files == NULL)
		cmd->redirection_files = lstnew;
	else
		ft_lstadd_back(&cmd->redirection_files, lstnew);
	*token = (*token)->next;
	sanitize_token(*token, msh);
	if (!(lstnew = ft_lstnew((*token)->content)))
		exit_print_err(strerror(errno));
	ft_lstadd_back(&cmd->redirection_files, lstnew);
}

void	making_cmd(t_msh *msh)
{
	t_cmd	*cmd;
	t_list	*token;

	cmd = get_new_cmd(NULL);
	msh->cmds = cmd;
	token = msh->tokens;
	while (token)
	{
		if (ft_strcmp(";", token->content) == 0 && token->next != NULL)
			cmd = get_new_cmd(cmd);
		else if (ft_strcmp("|", token->content) == 0)
		{
			cmd->type = TYPE_PIPE;
			cmd = get_new_cmd(cmd);
		}
		else if (ft_strcmp("<", token->content) == 0 ||
				ft_strcmp(">", token->content) == 0 ||
				ft_strcmp(">>", token->content) == 0)
			add_redirection_file(cmd, &token, msh);
		else
			add_args(cmd, token, msh);
		token = token->next;
	}
}
