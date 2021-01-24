/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 18:39:33 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/25 01:16:27 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "prompt.h"
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <signal.h>
# include "get_next_line.h"

# define FALSE				0
# define TRUE				1

# define SUCCESS			1
# define ERROR				-1

# define STDIN				0
# define STDOUT				1
# define STDERR				2

# define TOKEN_NONE			0
# define TOKEN_WORD			1
# define TOKEN_PIPE			2
# define TOKEN_SEMICOLON	3
# define TOKEN_REDIRECTION	4

# define TYPE_DEFAULT		0
# define TYPE_PIPE			1

# define DEFAULT_PATH		"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."

# define METACHARACTER		" \t\n|;<>"

extern				int g_exit_status;

typedef struct		s_cmd
{
	char			**args;
	int				length;
	int				type;
	t_list			*redirection_files;
	int				input_fd;
	int				output_fd;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}					t_cmd;

typedef struct		s_dict
{
	char			*key;
	char			*value;
}					t_dict;

typedef struct		s_msh
{
	int				c_option_flag;
	char			*launch_program_name;
	char			*program_name;
	t_list			*tokens;
	t_cmd			*cmds;
	t_dict			**env;
	int				env_len;
	char			*path;
	char			*pwd;
	int				unset_pwd_flag;
	char			*lstcmd;
}					t_msh;

typedef struct		s_piping_material
{
	int				cnt_of_pipes;
	int				*pipes;
	int				*cpid;
}					t_piping_material;


typedef void		(*t_builtin_executor)(t_msh *, t_cmd *);

/*
** builtins/
*/
void				*get_builtin_executor(char *cmd_name);
void				run_builtin_executor(t_msh *msh, t_cmd *cmd,
									t_builtin_executor builtin_executor,
									int is_in_child_process);
void				do_cd(t_msh *msh, t_cmd *cmd);
void				do_echo(t_msh *msh, t_cmd *cmd);
void				do_env(t_msh *msh, t_cmd *cmd);
void				do_exit(t_msh *msh, t_cmd *cmd);
void				do_export(t_msh *msh, t_cmd *cmd);
void				do_pwd(t_msh *msh, t_cmd *cmd);
void				do_unset(t_msh *msh, t_cmd *cmd);

/*
** executing/
*/
void				executing(t_msh *msh);
int					set_redirection_fd(t_msh *msh, t_cmd *cmd);
void				basic_executor(t_msh *msh, t_cmd *cmd);
void				create_process(t_msh *msh, t_cmd *cmd);
t_cmd				*piping(t_msh *msh, t_cmd *cmd);

/*
** init_free_msh/
*/
void				init_msh(char *program_name, t_msh *msh, char **env);
void				free_msh_member(t_msh *msh);

/*
** parsing/
*/
int					parsing(t_msh *msh, char *input);
void				split_token(char *input, t_list **tokens, char *separator);
int					check_token_valid(char *program_name, t_list *now);
void				making_cmd(t_msh *msh);
void				sanitize_token(t_list *token);
int					sanitize_env(
					char **res_str, char *og_str, t_msh *msh, int in_dquotes);
char				*get_env_replaced_input(t_msh *msh, char *input);

/*
** errors/
*/
void				exit_print_err(char *err_msg);
void				nonexit_print_err(char *err_msg);
int					print_syntax_err(
							char *program_name, char *token, int is_eof_err);
int					print_execute_err(
							char *program_name, char *token, char *err_msg);
void				print_shlvl_err(
							char *program_name, char *value);
void				print_identifier_err(
							char *program_name, char *token1, char *token2);

/*
** prompt/
*/
void				show_logo();
void				show_prompt();

/*
** utils/
*/
void				ft_double_free(void **ptr);
int					is_in_charset(char c, char *str);
int					ft_strcmp(const char *s1, const char *s2);
t_dict				*get_env_dict(t_dict **env, char *key);
void				set_env_dict(t_msh *msh, char *key, char *value);
void				unset_env_dict(t_msh *msh, char *key);
char				**ft_envjoin(t_dict **env, int env_len);
void				append_char_to_str(char **str, char c);
void				quick_sort_env(int left, int right, t_dict **env);
void				copy_env(t_msh *msh, t_dict **dst);
void				free_and_get_value(char **dst, char *src);
void				env_free(t_dict **env);
int					is_numeric_str(char *str);
int					is_numeric_long_str(char *str);
char				*insert_char_before_set(char *str, char *set, char ch);
int					is_fine_env_key(char *key);

/*
** signal.c
*/
void				init_signal(void);

#endif
