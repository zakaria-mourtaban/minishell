/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dolarcount.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:46:33 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 04:25:35 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_non_alpha_var(char *input, int i)
{
	if (input[i] == '$' && !ft_isalpha(input[i + 1]))
	{
		i++;
		while (input[i] != ' ' && input[i] != '\0' && input[i] != '\"')
			i++;
	}
	return (i);
}

static int	toggle_in_quote(char c, int inq)
{
	if (c == '"')
		return (!inq);
	return (inq);
}

static int	skip_single_quote_section(char *input, int i)
{
	if (input[i] == '\'')
	{
		i++;
		while (input[i] != '\'' && input[i] != '\0')
			i++;
		if (input[i] == '\'')
			i++;
	}
	return (i);
}

int	isquote(char c)
{
	return (c == '\'' || c == '\"');
}

int	dollarcount(char *input)
{
	int	i;
	int	count;
	int	inq;

	inq = 0;
	i = 0;
	count = 0;
	while (input[i] != '\0')
	{
		i = skip_non_alpha_var(input, i);
		if (input[i] == '$')
			count++;
		inq = toggle_in_quote(input[i], inq);
		i = skip_single_quote_section(input, i);
		i++;
	}
	return (count);
}

// int	dollarcount(char *input)
// {
// 	int	i;
// 	int	count;
// 	int	inq;

// 	inq = 0;
// 	i = 0;
// 	count = 0;
// 	while (input[i] != '\0')
// 	{
// 		if (input[i] == '$' && !isalpha(input[i + 1]))
// 		{
// 			i++;
// 			while (input[i] != ' ' && input[i] != '\0' && input[i] != '\"')
// 				i++;
// 			i++;
// 		}
// 		if (input[i] == '$')
// 			count++;
// 		if (input[i] == '"')
// 			inq = !inq;
// 		if (input[i] == '\'' && !inq)
// 		{
// 			i++;
// 			while (input[i] != '\'' && input[i] != '\0')
// 				i++;
// 			i++;
// 		}
// 		else
// 			i++;
// 	}
// 	return (count);
// }