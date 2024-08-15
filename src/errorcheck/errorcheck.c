/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorcheck.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:26:21 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/15 13:08:16 by zmourtab         ###   ########.fr       */
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
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->next;
	return (tmp);
}

int	checkfile(char *str)
{
	if (access(str, F_OK) != 0) {
		fprintf(stderr, "Error: File '%s' does not exist.\n", str);
		return (1);
	}
	if (access(str, W_OK) != 0) {
		fprintf(stderr, "Error: No write permission for file '%s'.\n", str);
		return (1);
	}
	return (0);
}

int	checkfileinperms(char *str)
{
	if (access(str, F_OK) != 0) {
		fprintf(stderr, "Error: File '%s' does not exist.\n", str);
		return (1);
	}
	if (access(str, R_OK) != 0) {
		fprintf(stderr, "Error: No read permission for file '%s'.\n", str);
		return (1);
	}
	return (0);
}

int	checkpipe(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD
		|| !getprev(token->previous)
		|| getprev(token->previous)->id != TOKEN_WORD)
		return (1);
	return (0);
}

int	checkfileoutappend(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	if (checkfile(getnext(token->next)->content))
		return (1);
	return (0);
}

int	checkheredoc(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	return (0);
}

int	checkfileout(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	if (checkfile(getnext(token->next)->content))
		return (1);
	return (0);
}

int	checkfilein(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	if (checkfileinperms(getnext(token->next)->content))
		return (1);
	return (0);
}

int	errorcheck(t_data *data)
{
	t_tokens *tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		switch (tmp->id) {
			case TOKEN_PIPE:
				if (checkpipe(tmp))
					return (1);
				break;
			case TOKEN_OUT_FILE:
			case TOKEN_OUT_A_FILE:
				if (checkfileout(tmp))
					return (1);
				break;
			case TOKEN_IN_FILE:
				if (checkfilein(tmp))
					return (1);
				break;
			case TOKEN_HEREDOC_EOF:
				if (checkheredoc(tmp))
					return (1);
				break;
			default:
				break;
		}
		tmp = tmp->next;
	}
	return (0);
}
