/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksyntaxerror.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:26:21 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/16 17:12:09 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tokens	*getprev(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->previous;
	return (tmp);
}

t_tokens	*getnext(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	if (token == NULL)
		return (NULL);
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->next;
	return (tmp);
}

int	checkpipe(t_tokens *token)
{
	if (!getnext(token->next) || (getnext(token->next)->id != TOKEN_WORD
			&& getnext(token->next)->id != TOKEN_COMMAND)
		|| !getprev(token->previous) || (getprev(token->next)->id != TOKEN_WORD
			&& getprev(token->next)->id != TOKEN_COMMAND))
		return (1);
	return (0);
}

int	checkfileoutappend(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	return (0);
}

int	checkheredoc(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_COMMAND)
		return (1);
	return (0);
}

int	checkfileout(t_tokens *token)
{
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_WORD
			&& next_token->id != TOKEN_COMMAND))
		return (1); // Error: No valid file or command after >
	return (0);     // No error
}

int	checkfilein(t_tokens *token)
{
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_WORD
			&& next_token->id != TOKEN_COMMAND))
		return (1); // Error: No valid file or command after <
	return (0);     // No error
}

void	printerror(t_tokens *token)
{
	if (getnext(token) == NULL)
		printf("bash: syntax error near unexpected token `newline'\n");
	else if (token->id == TOKEN_DIRECTORY)
		printf("bash: %s: Is a directory\n", getnext(token)->content);
	else
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			getnext(token)->content);
	}
}

int	isdirectory(t_tokens *token)
{
	DIR	*dir;

	dir = opendir(token->content);
	if (dir)
	{
		token->id = TOKEN_DIRECTORY;
		closedir(dir);
		return (1);
	}
	else
		return (0);
}

int	checksyntaxerror(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_PIPE)
		{
			if (checkpipe(tmp))
			{
				printerror(tmp);
				return (1);
			}
		}
		tmp = tmp->next;
	}
	tmp = data->cmdchain;
	while (tmp)
	{
		switch (tmp->id)
		{
		case TOKEN_COMMAND:
			if (isdirectory(tmp))
			{
				printerror(tmp);
				return (1);
			}
			break ;
		case (TOKEN_OUT_FILE):
			if (checkfileout(tmp))
			{
				printerror(tmp);
				return (1);
			}
			break ;
		case (TOKEN_OUT_A_FILE):
			if (checkfileout(tmp))
			{
				printerror(tmp);
				return (1);
			}
			break ;
		case TOKEN_IN_FILE:
			if (checkfilein(tmp))
			{
				printerror(tmp);
				return (1);
			}
			break ;
		case TOKEN_HEREDOC_EOF:
			if (checkheredoc(tmp))
			{
				printerror(tmp);
				return (1);
			}
			break ;
		default:
			break ;
		}
		tmp = tmp->next;
	}
	return (0);
}
