/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 02:27:18 by odib              #+#    #+#             */
/*   Updated: 2024/09/05 02:29:47 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tk_appendfo(char *input, t_tk *tk, t_data *data)
{
	append(&data->cmdchain, ">", TOKEN_OUT_FILE);
	tk->last_was_space = 0;
	(void)input;
}

void	tk_appendfin(char *input, t_tk *tk, t_data *data)
{
	append(&data->cmdchain, "<", TOKEN_IN_FILE);
	tk->last_was_space = 0;
	(void)input;
}

void	tk_redir(char *input, t_tk *tk, t_data *data)
{
	tk_bufpos(input, tk, data);
	if (tk_heredocument(input, tk, data))
		;
	else if (tk_outappend(input, tk, data))
		;
	else if (input[tk->i] == '>')
		tk_appendfo(input, tk, data);
	else if (input[tk->i] == '<')
		tk_appendfin(input, tk, data);
	else if (input[tk->i] == '|')
	{
		append(&data->cmdchain, "|", TOKEN_PIPE);
		tk->last_was_space = 0;
	}
	else if (input[tk->i] == ' ')
	{
		append(&data->cmdchain, " ", TOKEN_SPACE);
		tk->last_was_space = 0;
	}
}

void	tk_default(char *input, t_tk *tk, t_data *data)
{
	if (tk->last_was_space)
	{
		tk->buffer[0] = input[tk->i];
		tk->buffer[1] = '\0';
		append(&data->cmdchain, tk->buffer, get_delimiter_type(tk->buffer));
	}
	else
	{
		tk->buffer[tk->buf_i++] = input[tk->i];
	}
	tk->last_was_space = (input[tk->i] == ' ');
}

void	tk_bufposo(char *input, t_tk *tk, t_data *data)
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
	}
	(void)input;
}
