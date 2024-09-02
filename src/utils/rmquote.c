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

// int	numchar(char *input, char c)
// {
// 	int	i;
// 	int	count;

// 	count = 0;
// 	i = 0;
// 	while (input[i] != '\0')
// 	{
// 		while (input[i] == '\'')
// 		{
// 			i++;
// 			while (input[i] != '\'' && input[i] != '\0')
// 				i++;
// 			i++;
// 		}
// 		if (input[i] == c)
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// char	*removepair(char *buffer, int ic, int ci)
// {
// 	int		i;
// 	int		ni;
// 	char	*newbuffer;

// 	i = 0;
// 	ni = 0;
// 	newbuffer = malloc(sizeof(char) * (ft_strlen(buffer) - 2));
// 	if (newbuffer == NULL)
// 		return (NULL);
// 	while (buffer[i] != '\0')
// 	{
// 		if (i != ic && i != ci)
// 		{
// 			newbuffer[ni] = buffer[i];
// 			ni++;
// 		}
// 		i++;
// 	}
// 	newbuffer[ni] = '\0';
// 	free(buffer);
// 	return (newbuffer);
// }

// void	skipsingle(char *input, int *i)
// {
// 	if (input[*i] == '\'' && input[*i] != '\0')
// 	{
// 		(*i)++;
// 		while (input[*i] != '\'' && input[*i] != '\0')
// 			(*i)++;
// 	}
// }

// char	*loopquote(char *buffer, char c)
// {
// 	int	ci;
// 	int	ic;

// 	while (numchar(buffer, c) != 0)
// 	{
// 		ic = 0;
// 		ci = 0;
// 		while (buffer[ic] != c && buffer[ic] != '\0')
// 		{
// 			skipsingle(buffer, &ic);
// 			ic++;
// 		}
// 		if (buffer[ic] == '\0')
// 			break ;
// 		while (buffer[ic] != c && buffer[ic] != '\0')
// 		{
// 			skipsingle(buffer, &ic);
// 			ic++;
// 		}
// 		if (buffer[ic] == '\0')
// 			break ;
// 		ci = ic + 1;
// 		while (buffer[ci] != c && buffer[ci] != '\0')
// 		{
// 			skipsingle(buffer, &ci);
// 			ci++;
// 		}
// 		if (buffer[ci] == '\0')
// 			break ;
// 		buffer = removepair(buffer, ic, ci);
// 		printf("%s\n", buffer);
// 	}
// 	return (buffer);
// }

// char	*processstr(char *input, t_data *data)
// {
// 	char	*buffer;
// 	int		l;

// 	l = 0;
// 	buffer = ft_strdup(input);
// 	if ((numchar(buffer, '\"') % 2 != 0 && numchar(buffer, '\"') != 0))
// 		l++;
// 	if (l != 0)
// 	{
// 		data->cmd.status = 2;
// 		printf("error syntax should be erroring properly%d,%d\n", l,
// 			numchar(buffer, '\"'));
// 		return (NULL);
// 	}
// 	if (numchar(buffer, '\"') != 0)
// 		buffer = loopquote(buffer, '\"');
// 	(void)data;
// 	return (buffer);
// }

// char	*rmquote(char *input, t_data *data)
// {
// 	return (processstr(input, data));
// }