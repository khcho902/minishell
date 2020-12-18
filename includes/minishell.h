/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/18 11:57:48 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <sys/errno.h>

# define FALSE 0
# define TRUE 1

# define SUCCESS 1
# define ERROR -1

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define TYPE_DEFAULT 0
# define TYPE_PIPE 1

# define METACHARACTER " \t\n|;<>"

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	int				pipes[2];
	t_list			*redirection_files;
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
	t_cmd			*cmds;
	t_dict			**env;
	int				env_len;
	char			**path;
}					t_msh;

void				init_msh(char *program_name, t_msh *msh, char **env);
void				free_msh_member(t_msh *msh);

int					parsing(t_msh *msh, char *input);
void				split_token(char *input, t_list **tokens, int i);
int					check_token_valid(char *program_name, t_list *now);
void				making_cmd(t_msh *msh);

int					ft_strcmp(const char *s1, const char *s2);
int					is_in_charset(char c, char *str);
void				ft_double_free(char **ptr);
t_dict				*get_env_dict(t_dict **env, char *key);

void				exit_print_err(char *err_msg);
int					print_syntax_err(char *program_name, char *token);

#endif
