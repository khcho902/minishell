/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 23:49:13 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/errno.h>

#define MAX_PATH 256
#define FALSE 0
#define TRUE 1

#define SUCCESS 1
#define ERROR -1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define TYPE_DEFAULT 0
#define TYPE_PIPE 1

#define METACHARACTER " \t\n|;<>"

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	int				pipes[2];
	t_list			*redirection_file;
	struct s_cmd	*previous;
	struct s_cmd	*next;
}					t_cmd;

typedef struct		s_dict
{
	char			*key;
	char			*value;
}					t_dict;

typedef struct		s_msh
{
	char			*program_name;
	int				exit_status;
	t_list			*tokens;
	t_cmd			*cmd;
	t_dict			**env;
	int				env_len;
	char			**path;
}					t_msh;

void	exit_print_err(char *err_msg1, char *err_msg2, int exit_status)
{
	ft_putstr_fd(err_msg1, STDERR);
	ft_putstr_fd(" : ", STDERR);
	ft_putstr_fd(err_msg2, STDERR);
	ft_putstr_fd("\n", STDERR);
	exit(exit_status);
}

int		print_syntax_err(char *program_name, char *token)
{
	ft_putstr_fd("-", STDERR);
	ft_putstr_fd(program_name, STDERR);
	ft_putstr_fd(": syntax error near unexpected token `", STDERR);
	ft_putstr_fd(token, STDERR);
	ft_putstr_fd("'\n", STDERR);
	return (ERROR);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t max_len;
	size_t s1_len;
	size_t s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len > s2_len)
		max_len = s1_len;
	else
		max_len = s2_len;
	return ft_strncmp(s1, s2, max_len);
}

int		is_in_charset(char c, char *str)
{
	int idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == c)
			return (TRUE);
		idx++;
	}
	return (FALSE);
}

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

int		check_token_valid(t_msh *msh, t_list *now)
{
	int		before_type;

	before_type = 0;
	while (now)
	{
		if (((char*)now->content)[0] == '|' || ((char *)now->content)[0] == ';')
		{
			if (before_type != 1)
				return (print_syntax_err(msh->program_name, now->content));
			before_type = 2;
		}
		else if (((char *)now->content)[0] == '>' ||
				((char *)now->content)[0] == '<')
		{
			if (before_type == 3)
				return (print_syntax_err(msh->program_name, now->content));
			before_type = 3;
		}
		else
			before_type = 1;
		now = now->next;
	}
	if (before_type == 3)
		return (print_syntax_err(msh->program_name, "newline"));
	return (SUCCESS);
}

void	making_cmd(t_msh *msh)
{
	t_cmd *cmd;

	if (msh->tokens == NULL)
	{
		printf("msh->tokens NULL임\n");
		return ;
	}
	cmd = (t_cmd*)malloc(sizeof(t_cmd));
	cmd->args = (char**)malloc(sizeof(char*));
	cmd->args[0] = NULL;
	cmd->length = 0;
	cmd->type = TYPE_DEFAULT;
	cmd->pipes[0] = -1;
	cmd->pipes[1] = -1;
	cmd->redirection_file = NULL;
	cmd->previous = NULL;
	cmd->next = NULL;
	
	msh->cmd = cmd;

	t_list *token = msh->tokens;
	while(token)
	{
		if (ft_strcmp(";", token->content) == 0)
		{
			if (token->next != NULL)
			{
				t_cmd *tmp_cmd = (t_cmd*)malloc(sizeof(t_cmd));
				tmp_cmd->args = (char**)malloc(sizeof(char*));
				tmp_cmd->args[0] = NULL;
				tmp_cmd->length = 0;
				tmp_cmd->type = TYPE_DEFAULT;
				tmp_cmd->pipes[0] = -1;
				tmp_cmd->pipes[1] = -1;
				tmp_cmd->redirection_file = NULL;
				tmp_cmd->previous = NULL;
				tmp_cmd->next = NULL;

				cmd->next = tmp_cmd;
				tmp_cmd->previous = cmd;
				cmd = tmp_cmd;
				
			}
		}
		else if (ft_strcmp("|", token->content) == 0)
		{
			cmd->type = TYPE_PIPE;
			t_cmd *tmp_cmd = (t_cmd*)malloc(sizeof(t_cmd));
			tmp_cmd->args = (char**)malloc(sizeof(char*));
			tmp_cmd->args[0] = NULL;
			tmp_cmd->length = 0;
			tmp_cmd->type = TYPE_DEFAULT;
			tmp_cmd->pipes[0] = -1;
			tmp_cmd->pipes[1] = -1;
			tmp_cmd->redirection_file = NULL;
			tmp_cmd->previous = NULL;
			tmp_cmd->next = NULL;

			cmd->next = tmp_cmd;
			tmp_cmd->previous = cmd;
			cmd = tmp_cmd;
		}
		else if (ft_strcmp("<", token->content) == 0 ||
				ft_strcmp(">", token->content) == 0 ||
				ft_strcmp(">>", token->content) == 0)
		{
			if (cmd->redirection_file == NULL)
			{
				cmd->redirection_file = ft_lstnew(token->content);
				token = token->next;
				ft_lstadd_back(&cmd->redirection_file, ft_lstnew(token->content));
			}
			else
			{
				ft_lstadd_back(&cmd->redirection_file, ft_lstnew(token->content));
				token = token->next;
				ft_lstadd_back(&cmd->redirection_file, ft_lstnew(token->content));	
			}
		}
		else
		{
			char **tmp;

			tmp = (char**)malloc(sizeof(char*) * (cmd->length + 1 + 1));
			tmp[cmd->length + 1] = NULL;
			int i = 0;
			while (i < cmd->length)
			{
				tmp[i] = cmd->args[i];
				i++;
			}
			tmp[i] = token->content;
			if (cmd->args != NULL)
				free(cmd->args);
			cmd->args = tmp;
			cmd->length++;
		}
		token = token->next;
	}
}

int		parsing(t_msh *msh, char *input)
{
	int		res;

	if (msh == NULL || input == NULL)
		return (ERROR);
	res = SUCCESS;
	msh->tokens = NULL;
	split_token(input, &(msh->tokens), 0);

	printf("------------------------------\n");
	printf("lstsize : %d\n", ft_lstsize(msh->tokens));
	printf("tokens : %p\n", msh->tokens);
	t_list *now = msh->tokens;
	while (now)
	{
		printf("|%s|\n", now->content);
		now = now->next;
	}
	printf("------------------------------\n");
	
	if (check_token_valid(msh, msh->tokens) == SUCCESS)
	{
		making_cmd(msh);
	}
	else
		res = ERROR;
	return (res);
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
	msh->cmd = NULL;
	init_msh_env(msh, env);
	init_msh_path(msh);
}

int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	if (argc == 0)
		return (1);
	init_msh(argv[0], &msh, env);
	res = 1;
	while (res)
	{
		ft_putstr_fd(msh.program_name, STDOUT);
		ft_putstr_fd("$ ", STDOUT);
		res = get_next_line(STDIN, &input);
		if (res == -1)
			exit_print_err("Error", "get_next_line fail!", EXIT_FAILURE);
		else
		{
			
			if (parsing(&msh, input) == SUCCESS)
			{
				printf("execute cmds!!!\n");
			}
			else 
				printf("parsing error : no execute cmds!!!\n");

			t_cmd *cmd = msh.cmd;
			while (cmd)
			{
				printf("--------@@@@@@@@@@@@@-------------\n");
				int i = 0;
				while(cmd->args[i])
				{
					printf("|%s|", cmd->args[i]);
					i++;
				}
				printf("\n");
				printf("lenght : %d\n", cmd->length);
				printf("type : %d\n", cmd->type);
				printf("pipes : %d %d\n", cmd->pipes[0], cmd->pipes[1]);
				printf("  --redirection start--\n");
				t_list *tmp = cmd->redirection_file;
				while(tmp)
				{
					printf("%s\n", tmp->content);
					tmp = tmp->next;
				}
				printf("  --redirection end--\n");
				cmd = cmd->next;
				printf("--------@@@@@@@@@@@@@-------------\n");
			}

			printf("1 msh.cmd : %p\n", msh.cmd);
			printf("1 msh.tokens : %p\n", msh.tokens);

			while(msh.cmd)
			{
				free(msh.cmd->args);
				while(msh.cmd->redirection_file)
				{
					t_list *tmp = msh.cmd->redirection_file->next;
					free(msh.cmd->redirection_file);
					msh.cmd->redirection_file = tmp;
				}
				t_cmd *tmp_cmd = msh.cmd->next;
				free(msh.cmd);
				msh.cmd = tmp_cmd;
			}

			printf("msh.cmd : %p\n", msh.cmd);
			ft_lstclear(&msh.tokens, free);
			printf("msh.tokens : %p\n", msh.tokens);


			free(input);
		}
	}
	return (0);
}
