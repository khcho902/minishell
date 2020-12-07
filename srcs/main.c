/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/06 10:26:04 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_PATH 256
#define FALSE 0
#define TRUE 1

#define SUCCESS 1
#define ERROR -1

#define METACHARACTER " \t\n|;<>"

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	int				piles[2];
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
	int				exit_status;
	t_cmd			*cmd;
	t_dict			**envs;
	t_dict			**sorted_envs;
	int				envs_len;
}					t_msh;

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
		while(now)
		{
			printf("|%s|\n", now->content);
			now = now->next;
		}
	}
	ft_lstclear(&tokens, free);
}


int		init_msh(t_msh	*msh, char **env)
{
	int i;
	int	key_len;

	msh->exit_status = 0;
	msh->cmd = NULL;

	msh->envs_len = 0;
	while(env[msh->envs_len])
		msh->envs_len++;

	msh->envs = (t_dict**)malloc(sizeof(t_dict*) * (msh->envs_len + 1));
	msh->envs[msh->envs_len] = NULL;
	msh->sorted_envs = (t_dict**)malloc(sizeof(t_dict*) * (msh->envs_len + 1));
	msh->sorted_envs[msh->envs_len] = NULL;

	i = 0;
	while (env[i])
	{
		key_len = ft_strchr(env[i], '=') - env[i];
		msh->envs[i] = (t_dict*)malloc(sizeof(t_dict));
		msh->envs[i]->key = (char *)malloc(sizeof(char) * (key_len + 1));
		ft_strlcat(msh->envs[i]->key, env[i], key_len + 1);
		msh->envs[i]->value = ft_strdup(env[i] + key_len + 1);
		msh->sorted_envs[i] = msh->envs[i];
		i++;
	}


	t_dict* tmp;
	
	for (int j = 0; j < msh->envs_len; j++)
	{
		for (int k = 0; k < msh->envs_len - 1; k++)
		{
			int max_len = ft_strlen(msh->sorted_envs[k]->key);
			if (max_len < (int)ft_strlen(msh->sorted_envs[k + 1]->key))
				max_len = ft_strlen(msh->sorted_envs[k + 1]->key);
			if (ft_strncmp(msh->sorted_envs[k]->key, msh->sorted_envs[k + 1]->key, max_len) > 0)
			{
				tmp = msh->sorted_envs[k];
				msh->sorted_envs[k] = msh->sorted_envs[k + 1];
				msh->sorted_envs[k + 1] = tmp;
			}
		}
	}
	return (1);
}

int		main(int argc, char** argv, char** env)
{
	char	*input;
	int		res;
	t_msh	msh;

	if (argc == 0 && argv == NULL && env == NULL) //
		return (1);
	printf("===========================\n");
	int i = 0;
	while(argv[i])
	{	
		printf("%s\n", argv[i]);
		i++;
	}	
	printf("===========================\n");


	init_msh(&msh, env);


	res = 1;
	while (res)
	{
		ft_putstr_fd("minishell$ ", STDOUT_FILENO);
		res = get_next_line(STDIN_FILENO, &input);
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
