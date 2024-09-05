/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 06:03:06 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 06:16:03 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_tokens *head)
{
	t_tokens	*current;
	t_tokens	*next;

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
	int	single_quote_count;
	int	double_quote_count;

	single_quote_count = 0;
	double_quote_count = 0;
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

void	fixupcommand(t_data *data)
{
	int			foundcmd;
	t_tokens	*tmp;

	foundcmd = 0;
	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_COMMAND)
			foundcmd = 1;
		if (tmp->id == TOKEN_PIPE)
			foundcmd = 0;
		if (tmp->id == TOKEN_WORD && foundcmd == 0)
		{
			foundcmd = 1;
			tmp->id = TOKEN_COMMAND;
		}
		tmp = tmp->next;
	}
}

void	fixuptoken(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		if ((tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE
				|| tmp->id == TOKEN_HEREDOC_EOF || tmp->id == TOKEN_IN_FILE))
		{
			tmp = tmp->next;
			if (tmp && tmp->id == TOKEN_SPACE)
				tmp = tmp->next;
			if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND))
				tmp->id = TOKEN_FILE;
		}
		if (tmp)
			tmp = tmp->next;
		if (tmp && tmp->id == TOKEN_SPACE)
			tmp = tmp->next;
	}
	fixupcommand(data);
}

t_tokens	*getcommandsfirst(t_command **current_cmd, t_tokens *tmp)
{
	if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND))
	{
		if (!(*current_cmd))
		{
			*current_cmd = create_command_node();
			if (!*current_cmd)
				return (NULL);
		}
		add_argument(*current_cmd, tmp->content);
		if (tmp->error == 1)
			(*current_cmd)->error = 1;
	}
	if (tmp)
		return (tmp->next);
	return (NULL);
}
