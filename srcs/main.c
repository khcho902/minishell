/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/08 21:47:47 by kycho            ###   ########.fr       */
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

#define METACHARACTER " \t\n|;<>"

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	int				pipes[2];
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

void	split_token(char *input, t_list **tokens)
{
	int i;
	int len;

	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == 0)
			break ;
		len = 1;
		if (is_in_charset(input[i], METACHARACTER))
		{
			if (input[i] == '>' && input[i + 1] == '>')
				len++;
		}
		else
			split_token_sub(input, &i, &len);
		if (*tokens == NULL)
			*tokens = ft_lstnew(ft_substr(input, i, len));
		else
			ft_lstadd_back(tokens, ft_lstnew(ft_substr(input, i, len)));
		i += len;
	}
}

int		check_token_valid(t_list **tokens)
{
	t_list	*now;
	int		before_type;

	before_type = 0;
	now = *tokens;
	while (now)
	{
		if (((char*)now->content)[0] == '|' || ((char *)now->content)[0] == ';')
		{
			if (before_type != 1)
			{
				printf("-bash: syntax error near unexpected token `%s'\n", now->content);
				return (-1);
			}
			before_type = 2;
		}
		else if (((char *)now->content)[0] == '>' || ((char *)now->content)[0] == '<')
		{
			if (before_type == 3)
			{
				printf("-bash: syntax error near unexpected token `%s'\n", now->content);
				return (-1);
			}
			before_type = 3;
		}
		else
		{
			before_type = 1;
		}
		now = now->next;
	}
	if (before_type == 3)
	{
		printf("-bash: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	return (1);
}

void	parsing(t_msh *msh, char *input)
{
	t_list	*tokens;

	if (msh == NULL)
		return ;
	printf("input : %s\n", input);
	tokens = NULL;
	split_token(input, &tokens);
	if (check_token_valid(&tokens) == -1)
	{
		printf("error\n");
	}
	else
	{
		printf("%d\n", ft_lstsize(tokens));
		t_list *now = tokens;
		while (now)
		{
			printf("|%s|\n", now->content);
			now = now->next;
		}
	}
	ft_lstclear(&tokens, free);
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
	msh->program_name = program_name;
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

	/*
	printf("----------------------------------------------------\n");
	int i = 0; 
	while (msh.env[i])
	{
		printf("%s=%s\n", msh.env[i]->key, msh.env[i]->value);
		i++;
	}
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	i = 0;
	while (msh.path[i])
	{
		printf("%s\n", msh.path[i]);
		i++;
	}
	printf("----------------------------------------------------\n");
	*/

	res = 1;
	while (res)
	{
		ft_putstr_fd("minishell$ ", STDOUT);
		res = get_next_line(STDIN, &input);
		if (res == -1)
		{
			exit(0);  // TODO : 에러발생 출력
		}
		else 
		{
			parsing(&msh, input);
			free(input);
		}
	}
	return (0);
}
