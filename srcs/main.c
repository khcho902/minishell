/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiseo <jiseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 16:02:10 by jiseo             #+#    #+#             */
/*   Updated: 2021/01/09 15:44:21 by kycho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_input_valid(char *program_name, char *input)
{
	int in_dquotes;
	int i;

	in_dquotes = FALSE;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\\')
			i++;
		else if (in_dquotes == FALSE && input[i] == '\'' )
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
			if (input[i] != '\'')
				return (print_syntax_err(program_name, "'", TRUE));
		}
		else if (input[i] == '"')
			in_dquotes ^= TRUE;
		i++;
	}
	if (in_dquotes == TRUE)
		return (print_syntax_err(program_name, "\"", TRUE));
	return (SUCCESS);
}


void	split_job_sub(char *input, int *i, int *len)
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
		if (is_in_charset(input[*i + *len], ";")
				&& input[*i + *len - 1] != '\\')
		{
			break ;
		}
		(*len)++;
	}
}

void	split_job(char *input, t_list **jobs)
{
	int		i;
	int		len;
	char	*str;
	t_list	*lstnew;

	i = 0;
	while (input[i])
	{
		len = 1;
		if (!is_in_charset(input[i], ";"))
			split_job_sub(input, &i, &len);
		else 
		{
			i++;
			continue;
		}

		if (!(str = ft_substr(input, i, len)))
			exit_print_err(strerror(errno));

		if (!(lstnew = ft_lstnew(str)))
			exit_print_err(strerror(errno));

		if (*jobs == NULL)
			*jobs = lstnew;
		else
			ft_lstadd_back(jobs, lstnew);
		i += len;
	}
}



int		main(int argc, char **argv, char **env)
{
	char	*input;
	int		res;
	t_msh	msh;

	init_msh(argv[0], &msh, env);
	init_signal();

	/****************************************************/
	if (argc == 3 && ft_strcmp("-c", argv[1]) == 0)
	{
		input = argv[2];
		/*--------------------------------------------------------------------*/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			return (msh.exit_status & 255);
		}


		t_list *tokens = NULL;
		split_token(input, &tokens, METACHARACTER, 0);
		if (check_token_valid(msh.program_name, tokens) == ERROR)
		{
			msh.exit_status = 258;
			ft_lstclear(&(tokens), free);
			return (msh.exit_status & 255);
		}
		ft_lstclear(&(tokens), free);



		t_list *jobs = NULL;
		t_list *job_now = NULL;
		split_job(input, &jobs);
		job_now = jobs;
		while (job_now)
		{
			if (parsing(&msh, (char *)job_now->content) == SUCCESS)
				executing(&msh);
			free_msh_member(&msh);

			job_now = job_now->next;
		}
		ft_lstclear(&(jobs), free);

		return (msh.exit_status & 255);
	}
	/****************************************************/

	show_logo();
	res = argc;
	while (res)
	{
		show_prompt(&msh);
		if ((res = get_next_line(STDIN, &input)) == -1)
			exit_print_err("get_next_line fail");
		/*--------------------------------------------------------------------*/
		if (check_input_valid(msh.program_name, input) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			continue;
		}


		t_list *tokens = NULL;
		split_token(input, &tokens, METACHARACTER, 0);
		if (check_token_valid(msh.program_name, tokens) == ERROR)
		{
			msh.exit_status = 258;
			free(input);
			ft_lstclear(&(tokens), free);
			continue;
		}
		ft_lstclear(&(tokens), free);



		t_list *jobs = NULL;
		t_list *job_now = NULL;
		split_job(input, &jobs);
		job_now = jobs;
		while (job_now)
		{
			if (parsing(&msh, (char *)job_now->content) == SUCCESS)
			{
			/*	
				t_cmd *cmd = msh.cmds;
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
					printf("  --redirection start--\n");
					t_list *tmp = cmd->redirection_files;
					while(tmp)
					{
						printf("%s\n", tmp->content);
						tmp = tmp->next;
					}
					printf("  --redirection end--\n");
					cmd = cmd->next;
					printf("--------@@@@@@@@@@@@@-------------\n");
				}

				exit(0);
*/

				executing(&msh);
			}
			free_msh_member(&msh);

			job_now = job_now->next;
		}
		ft_lstclear(&(jobs), free);


		free(input);
	}
	return (msh.exit_status & 255);
}
