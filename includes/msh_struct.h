/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 02:36:28 by jiseo             #+#    #+#             */
/*   Updated: 2020/12/15 03:25:17 by jiseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_STRUCT_H
# define MSH_STRUCT_H

# define FALSE 0
# define TRUE 1

# define SUCCESS 1
# define ERROR -1

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define EXEC_IDX 0

# define TYPE_DEFAULT 0
# define TYPE_PIPE 1

# define METACHARACTER " \t\n|;<>"

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	int				pipe[2];
	t_list			*redirection_file;
	struct s_cmd	*previous;
	struct s_cmd	*next;
}					t_cmd;

typedef struct		s_msh
{
	char			*input;
	t_list			*env_list;
	char			**cmd_list;
	int				cmd_idx;
	int				cmd_key;
	char			*prompt;
	char			*wd;
}					t_msh;

/*
typedef struct		s_msh
{
	char			*program_name;
	int				exit_status;
	t_list			*tokens;
	t_cmd			*cmd;
	t_dict			**env;
	int				env_len;
	char			**path;
}					t_msh; */

typedef struct		s_dict
{
	char			*key;
	char			*value;
}					t_dict;

enum
{
	k_cd = 1,
	k_echo = 2,
	k_env = 3,
	k_export = 4,
	k_pwd = 5,
	k_unset = 6,
	k_exit = 7
};
#endif
