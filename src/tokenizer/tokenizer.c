/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:07:10 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 18:00:26 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tokens	*newnode(char *data, int type)
{
	t_tokens	*ptr;

	ptr = malloc(sizeof(t_tokens));
	if (ptr == NULL)
		return (NULL);
	ptr->content = ft_strdup(data);
	if (ptr->content == NULL)
	{
		free(ptr);
		return (NULL);
	}
	ptr->id = type;
	ptr->next = NULL;
	ptr->previous = NULL;
	return (ptr);
}

void	append(t_tokens **cmds, char *data, int type)
{
	t_tokens	*new_node;
	t_tokens	*tmp;

	new_node = NULL;
	if (*cmds == NULL)
	{
		new_node = newnode(data, type);
		new_node->previous = newnode("START", TOKEN_START);
		*cmds = new_node;
		return ;
	}
	tmp = *cmds;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	if ((tmp->id == TOKEN_COMMAND || tmp->id == TOKEN_WORD)
		&& (type == TOKEN_WORD || type == TOKEN_COMMAND))
		tmp->content = ft_strjoingnl(tmp->content, data);
	else
	{
		new_node = newnode(data, type);
		tmp->next = new_node;
		new_node->previous = tmp;
	}
}

void	printcmds(t_data *data)
{
	t_tokens	*tmp;
	const char	*type_names[] = {"DEFID", "WORD", "INFILE", "OUTFILE",
			"HEREDOC", "OUTAPPEND", "COMMAND", "OPERATOR", "PIPE", "SPACE"};

	tmp = data->cmdchain;
	while (tmp != NULL)
	{
		printf("[%s] %s\n", type_names[tmp->id], tmp->content);
		tmp = tmp->next;
	}
}

e_token	get_delimiter_type(char *str)
{
	if (ft_strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	else if (ft_strcmp(str, "<") == 0)
		return (TOKEN_IN_FILE);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_OUT_FILE);
	else if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC_EOF);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_OUT_A_FILE);
	return (TOKEN_WORD);
}

typedef struct s_tokenizer_state
{
	char	*buffer;
	int		buf_i;
	char	quote;
	int		last_was_space;
	int		foundcmd;
	t_data	*data;
	char	*input;
	int		i;
}			t_tokenizer_state;

void		handle_quote(t_tokenizer_state *state);
void		handle_special_characters(t_tokenizer_state *state);
void		handle_regular_characters(t_tokenizer_state *state);
void		finalize_buffer(t_tokenizer_state *state);

void	tokenizer(char *input, t_data *data)
{
	t_tokenizer_state	state;

	state.buffer = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	state.buf_i = 0;
	state.quote = 0;
	state.last_was_space = 0;
	state.foundcmd = 0;
	state.data = data;
	state.input = input;
	state.i = 0;
	data->cmdchain = NULL;
	while (input[state.i] != '\0')
	{
		if (state.quote)
		{
			handle_quote(&state);
		}
		else if (input[state.i] == '\'' || input[state.i] == '\"')
		{
			handle_quote(&state);
		}
		else if (ft_strchr("|<> ", input[state.i]))
		{
			handle_special_characters(&state);
		}
		else
		{
			handle_regular_characters(&state);
		}
		if (input[state.i] == '|')
			state.foundcmd = 0;
		state.i++;
	}
	finalize_buffer(&state);
	free(state.buffer);
}

void	handle_quote(t_tokenizer_state *state)
{
	state->buffer[state->buf_i++] = state->input[state->i];
	if (state->input[state->i] == state->quote)
	{
		state->quote = 0;
		state->buffer[state->buf_i] = '\0';
		if (!state->foundcmd)
		{
			append(&state->data->cmdchain, state->buffer, TOKEN_COMMAND);
			state->foundcmd = 1;
		}
		else
			append(&state->data->cmdchain, state->buffer, TOKEN_WORD);
		state->buf_i = 0;
	}
	state->last_was_space = 0;
}

void	handle_special_characters(t_tokenizer_state *state)
{
	if (state->buf_i > 0)
	{
		state->buffer[state->buf_i] = '\0';
		if (!state->foundcmd)
		{
			append(&state->data->cmdchain, state->buffer, TOKEN_COMMAND);
			state->foundcmd = 1;
		}
		else
			append(&state->data->cmdchain, state->buffer, TOKEN_WORD);
		state->buf_i = 0;
	}
	if (state->input[state->i] == '<' && state->input[state->i + 1] == '<')
	{
		append(&state->data->cmdchain, "<<", TOKEN_HEREDOC_EOF);
		state->i++;
		state->last_was_space = 0;
	}
	else if (state->input[state->i] == '>' && state->input[state->i + 1] == '>')
	{
		append(&state->data->cmdchain, ">>", TOKEN_OUT_A_FILE);
		state->i++;
		state->last_was_space = 0;
	}
	else if (state->input[state->i] == '>')
	{
		append(&state->data->cmdchain, ">", TOKEN_OUT_FILE);
		state->last_was_space = 0;
	}
	else if (state->input[state->i] == '<')
	{
		append(&state->data->cmdchain, "<", TOKEN_IN_FILE);
		state->last_was_space = 0;
	}
	else if (state->input[state->i] == '|')
	{
		append(&state->data->cmdchain, "|", TOKEN_PIPE);
		state->last_was_space = 0;
	}
	else if (state->input[state->i] == ' ')
	{
		append(&state->data->cmdchain, " ", TOKEN_SPACE);
		state->last_was_space = 0;
	}
}

void	handle_regular_characters(t_tokenizer_state *state)
{
	if (state->last_was_space)
	{
		state->buffer[0] = state->input[state->i];
		state->buffer[1] = '\0';
		append(&state->data->cmdchain, state->buffer,
			get_delimiter_type(state->buffer));
	}
	else
	{
		state->buffer[state->buf_i++] = state->input[state->i];
	}
	state->last_was_space = (state->input[state->i] == ' ');
}

void	finalize_buffer(t_tokenizer_state *state)
{
	if (state->buf_i > 0)
	{
		state->buffer[state->buf_i] = '\0';
		if (!state->foundcmd)
		{
			append(&state->data->cmdchain, state->buffer, TOKEN_COMMAND);
			state->foundcmd = 1;
		}
		else
			append(&state->data->cmdchain, state->buffer, TOKEN_WORD);
	}
}
