/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/16 10:11:02 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_token_sub(char *input, int *i, int *len)
{
	while (input[*i + *len])
	{
		if (input[*i + *len - 1] == '"'
				&& (*i + *len - 1 == 0 || input[*i + *len - 2] != '\\'))
		{
			while (!(input[*i + *len] == '"' && input[*i + *len - 1] != '\\')
					&& input[*i + *len] != 0)
				(*len)++;
			(*len)++;
		}
		if (input[*i + *len - 1] == '\''
				&& (*i + *len - 1 == 0 || input[*i + *len - 2] != '\\'))
		{
			while (!(input[*i + *len] == '\'') && input[*i + *len] != 0)
				(*len)++;
			(*len)++;
		}
		if (is_in_charset(input[*i + *len], METACHARACTER)
				&& input[*i + *len - 1] != '\\')
		{
			break ;
		}
		(*len)++;
	}
}

void	split_token(char *input, t_list **tokens, int i)
{
	int		len;
	char	*str;
	t_list	*lstnew;

	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		len = 1;
		if (!is_in_charset(input[i], METACHARACTER))
			split_token_sub(input, &i, &len);
		else if (ft_strncmp(&input[i], ">>", 2) == 0)
			len++;
		if (!(str = ft_substr(input, i, len)))
			exit_print_err("Error", strerror(errno), EXIT_FAILURE);
		if (!(lstnew = ft_lstnew(str)))
			exit_print_err("Error", strerror(errno), EXIT_FAILURE);
		if (*tokens == NULL)
			*tokens = lstnew;
		else
			ft_lstadd_back(tokens, lstnew);
		i += len;
	}
}

int		check_token_valid(char *program_name, t_list *now)
{
	int		before_type;

	before_type = 0;
	while (now)
	{
		if (((char*)now->content)[0] == '|' || ((char *)now->content)[0] == ';')
		{
			if (before_type != 1)
				return (print_syntax_err(program_name, now->content));
			before_type = 2;
		}
		else if (((char *)now->content)[0] == '>' ||
				((char *)now->content)[0] == '<')
		{
			if (before_type == 3)
				return (print_syntax_err(program_name, now->content));
			before_type = 3;
		}
		else
			before_type = 1;
		now = now->next;
	}
	if (before_type == 3)
		return (print_syntax_err(program_name, "newline"));
	return (SUCCESS);
}

t_cmd	*get_new_cmd(t_cmd *previous)
{
	t_cmd	*new_cmd;

	if (!(new_cmd = (t_cmd*)malloc(sizeof(t_cmd))))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
	if (!(new_cmd->args = (char**)malloc(sizeof(char*))))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
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

void	add_args(t_cmd *cmd, char *token_content)
{
	int		i;
	char	**tmp;

	if (!(tmp = (char**)malloc(sizeof(char*) * (cmd->length + 2))))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
	tmp[cmd->length + 1] = NULL;
	i = 0;
	while (i < cmd->length)
	{
		tmp[i] = cmd->args[i];
		i++;
	}
	tmp[i] = token_content;
	if (cmd->args != NULL)
		free(cmd->args);
	cmd->args = tmp;
	cmd->length++;
}

void	add_redirection_file(t_cmd *cmd, t_list **token)
{
	t_list *lstnew;

	if (!(lstnew = ft_lstnew((*token)->content)))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
	if (cmd->redirection_files == NULL)
		cmd->redirection_files = lstnew;
	else
		ft_lstadd_back(&cmd->redirection_files, lstnew);
	*token = (*token)->next;
	if (!(lstnew = ft_lstnew((*token)->content)))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
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
			add_redirection_file(cmd, &token);
		else
			add_args(cmd, token->content);
		token = token->next;
	}
}

int		parsing(t_msh *msh, char *input)
{
	if (msh == NULL || input == NULL)
		return (ERROR);
	msh->tokens = NULL;
	split_token(input, &(msh->tokens), 0);

	printf("------------------------------\n");
	printf("lstsize : %d\n", ft_lstsize(msh->tokens));
	printf("tokens : %p\n", msh->tokens);
	t_list *now = msh->tokens;
	while (now)
	{
		printf("|%s|\n", (char *)now->content);
		now = now->next;
	}
	printf("------------------------------\n");

	if (check_token_valid(msh->program_name, msh->tokens) == ERROR)
		return (ERROR);
	making_cmd(msh);
	return (SUCCESS);
}

void	init_msh_env(t_msh *msh, char **env)
{
	int i;
	int	key_len;

	msh->env_len = 0;
	while (env[msh->env_len])
		msh->env_len++;
	if (!(msh->env = (t_dict**)malloc(sizeof(t_dict*) * (msh->env_len + 1))))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
	msh->env[msh->env_len] = NULL;
	i = 0;
	while (env[i])
	{
		key_len = ft_strchr(env[i], '=') - env[i];
		if (!(msh->env[i] = (t_dict*)malloc(sizeof(t_dict))))
			exit_print_err("Error", strerror(errno), EXIT_FAILURE);
		if (!(msh->env[i]->key = (char *)malloc(sizeof(char) * (key_len + 1))))
			exit_print_err("Error", strerror(errno), EXIT_FAILURE);
		ft_strlcat(msh->env[i]->key, env[i], key_len + 1);
		if (!(msh->env[i]->value = ft_strdup(env[i] + key_len + 1)))
			exit_print_err("Error", strerror(errno), EXIT_FAILURE);
		i++;
	}
}

void	init_msh_path(t_msh *msh)
{
	int		i;
	size_t	len;

	i = 0;
	while (msh->env[i])
	{
		len = ft_strlen("PATH");
		if (ft_strlen(msh->env[i]->key) > len)
			len = ft_strlen(msh->env[i]->key);
		if (ft_strncmp(msh->env[i]->key, "PATH", len) == 0)
		{
			msh->path = ft_split(msh->env[i]->value, ':');
			if (msh->path == NULL)
				exit_print_err("Error", strerror(errno), EXIT_FAILURE);
			break ;
		}
		i++;
	}
}

void	init_msh(char *program_name, t_msh *msh, char **env)
{
	int i;

	i = ft_strlen(program_name) - 1;
	while (i >= 0 && program_name[i] != '/')
		i--;
	if (!(msh->program_name = ft_strdup(program_name + i + 1)))
		exit_print_err("Error", strerror(errno), EXIT_FAILURE);
	msh->exit_status = 0;
	msh->cmds = NULL;
	init_msh_env(msh, env);
	init_msh_path(msh);
}
