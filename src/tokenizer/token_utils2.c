/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 02:25:05 by odib              #+#    #+#             */
/*   Updated: 2024/09/05 02:30:29 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tk_quote(char *input, t_tk *tk, t_data *data)
{
	tk->buffer[tk->buf_i++] = input[tk->i];
	if (input[tk->i] == tk->quote)
	{
		tk->quote = 0;
		tk->buffer[tk->buf_i] = '\0';
		if (!tk->foundcmd)
		{
			append(&data->cmdchain, tk->buffer, TOKEN_COMMAND);
			tk->foundcmd = 1;
		}
		else
			append(&data->cmdchain, tk->buffer, TOKEN_WORD);
		tk->buf_i = 0;
	}
	tk->last_was_space = 0;
}

void	tk_isquote(char *input, t_tk *tk, t_data *data)
{
	if (tk->buf_i > 0)
	{
		tk->buffer[tk->buf_i] = '\0';
		if (!tk->foundcmd && data->cmdchain
			&& data->cmdchain->previous->id != TOKEN_START
			&& data->cmdchain->previous->id != TOKEN_COMMAND)
		{
			append(&data->cmdchain, tk->buffer, TOKEN_COMMAND);
			tk->foundcmd = 1;
		}
		else
			append(&data->cmdchain, tk->buffer, TOKEN_WORD);
		tk->buf_i = 0;
	}
	tk->quote = input[tk->i];
	tk->buffer[tk->buf_i++] = input[tk->i];
	tk->last_was_space = 0;
}

void	tk_bufpos(char *input, t_tk *tk, t_data *data)
{
	if (tk->buf_i > 0)
	{
		tk->buffer[tk->buf_i] = '\0';
		if (!tk->foundcmd)
		{
			append(&data->cmdchain, tk->buffer, TOKEN_COMMAND);
			tk->foundcmd = 1;
		}
		else
			append(&data->cmdchain, tk->buffer, TOKEN_WORD);
		tk->buf_i = 0;
	}
	(void)input;
}

int	tk_heredocument(char *input, t_tk *tk, t_data *data)
{
	if (input[tk->i] == '<' && input[tk->i + 1] == '<')
	{
		append(&data->cmdchain, "<<", TOKEN_HEREDOC_EOF);
		tk->i++;
		tk->last_was_space = 0;
		return (1);
	}
	return (0);
}

int	tk_outappend(char *input, t_tk *tk, t_data *data)
{
	if (input[tk->i] == '>' && input[tk->i + 1] == '>')
	{
		append(&data->cmdchain, ">>", TOKEN_OUT_A_FILE);
		tk->i++;
		tk->last_was_space = 0;
		return (1);
	}
	return (0);
}
