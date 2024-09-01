/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/01 17:06:09 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 17:06:09 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	free_tokens(t_tokens *head)
{
	t_tokens *current;
	t_tokens *next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

int	check_quotes(const char *str)
{
	int single_quote_count = 0;
	int double_quote_count = 0;

	while (*str)
	{
		if (*str == '\'')
			single_quote_count++;
		else if (*str == '"')
			double_quote_count++;
		str++;
	}
	if (single_quote_count % 2 == 0 && double_quote_count % 2 == 0)
		return (0);
	else
		return (1);
}

void	initcmd(char *input, char **env, t_data *data)
{
	t_command *command;

	if (ft_strlen(input) == 0)
		return ;
	tokenizer(input, data);

	concatenvtoken(data);

	printcmds(data);
	remove_quotes(data->cmdchain);
	if (!checksyntaxerror(data))
	{
		command = parse_tokens(data->cmdchain);

		execute_pipeline(command, data);
		free_command_list(command);
	}
	free(input);
	free_cmdchain(data->cmdchain);
	data->cmd.status = data->cmd.status % 255;
	(void)env;
	(void)command;
	(void)data;
}
