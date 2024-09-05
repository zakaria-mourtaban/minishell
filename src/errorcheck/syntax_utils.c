/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 02:01:47 by odib              #+#    #+#             */
/*   Updated: 2024/09/05 14:20:44 by zmourtab         ###   ########.fr       */
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
	if (token && token->next && token->next->id == TOKEN_SPACE)
		token = token->next;
	if (token && token->next)
		return (token->next);
	return (NULL);
}

int	checkpipe(t_tokens *token)
{
	if (!getnext(token->next) || (getnext(token->next)->id != TOKEN_WORD
			&& getnext(token->next)->id != TOKEN_COMMAND)
		|| !getprev(token->previous)
		|| (getprev(token->previous)->id != TOKEN_WORD
			&& getprev(token->previous)->id != TOKEN_COMMAND))
	{
		if (getprev(token->previous))
			getprev(token->previous)->error = 1;
		if (getnext(token->next))
			getnext(token->next)->error = 1;
		return (1);
	}
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
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_FILE))
		return (1);
	return (0);
}
