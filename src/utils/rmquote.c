/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rmquote.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:42:37 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/08 22:37:37 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_quotes(t_tokens *tokens)
{
	char	*src;
	char	*dst;
	char	quote;

	while (tokens)
	{
		quote = 0;
		src = tokens->content;
		dst = tokens->content;
		while (*src)
		{
			if (!quote && (*src == '\'' || *src == '\"'))
				quote = *src;
			else if (quote && *src == quote)
				quote = 0;
			else
			{
				*dst = *src;
				dst++;
			}
			src++;
		}
		*dst = '\0';
		tokens = tokens->next;
	}
}
