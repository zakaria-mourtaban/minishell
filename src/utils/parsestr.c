/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsestr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 06:58:20 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 07:04:33 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	remove_quotes(t_tokens *tokens)
// {
// 	char	*src;
// 	char	*dst;
// 	char	quote;

// 	while (tokens)
// 	{
// 		quote = 0;
// 		src = tokens->content;
// 		dst = tokens->content;
// 		while (*src)
// 		{
// 			if (!quote && (*src == '\'' || *src == '\"'))
// 				quote = *src;
// 			else if (quote && *src == quote)
// 				quote = 0;
// 			else
//             {
// 				*dst = *src;
//                 dst++;
//             }
// 			src++;
// 		}
// 		*dst = '\0';
// 		tokens = tokens->next;
// 	}
// // }

// void	specify_token_cmd(t_tokens *token)
// {
// 	while (token)
// 	{
// 		while (token && token->id != TOKEN_COMMAND && token->id != TOKEN_WORD)
// 		{
// 			token = token->next;
// 		}
// 		if (!token)
// 			return ;
// 		token->id = TOKEN_COMMAND;
// 		while (token && token->id != TOKEN_PIPE)
// 		{
// 			token = token->next;
// 		}
// 	}
// }
