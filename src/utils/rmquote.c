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

int	numchar(char *input, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == '\'')
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
			i++;
		}
		if (input[i] == c)
			count++;
		i++;
	}
	return (count);
}

char	*removepair(char *buffer, int ic, int ci)
{
	int		i;
	int		ni;
	char	*newbuffer;

	i = 0;
	ni = 0;
	newbuffer = malloc(sizeof(char) * (ft_strlen(buffer) - 2));
	if (newbuffer == NULL)
		return (NULL);
	while (buffer[i] != '\0')
	{
		if (i != ic && i != ci)
		{
			newbuffer[ni] = buffer[i];
			ni++;
		}
		i++;
	}
	newbuffer[ni] = '\0';
	free(buffer);
	return (newbuffer);
}

void	skipsingle(char *input, int *i)
{
	if (input[*i] == '\'' && input[*i] != '\0')
	{
		(*i)++;
		while (input[*i] != '\'' && input[*i] != '\0')
			(*i)++;
	}
}

char	*loopquote(char *buffer, char c)
{
	int	ci;
	int	ic;

	while (numchar(buffer, c) != 0)
	{
		ic = 0;
		ci = 0;
		while (buffer[ic] != c && buffer[ic] != '\0')
		{
			skipsingle(buffer, &ic);
			ic++;
		}
		if (buffer[ic] == '\0')
			break ;
		while (buffer[ic] != c && buffer[ic] != '\0')
		{
			skipsingle(buffer, &ic);
			ic++;
		}
		if (buffer[ic] == '\0')
			break ;
		ci = ic + 1;
		while (buffer[ci] != c && buffer[ci] != '\0')
		{
			skipsingle(buffer, &ci);
			ci++;
		}
		if (buffer[ci] == '\0')
			break ;
		buffer = removepair(buffer, ic, ci);
		printf("%s\n", buffer);
	}
	return (buffer);
}

char	*processstr(char *input, t_data *data)
{
	char	*buffer;
	int		l;

	l = 0;
	buffer = ft_strdup(input);
	// if (numchar(buffer, '\'') % 2 != 0 && numchar(buffer, '\'') != 0)
	// 	l++;
	if ((numchar(buffer, '\"') % 2 != 0 && numchar(buffer, '\"') != 0))
		l++;
	if (l != 0)
	{
		singalint = 2;
		printf("error syntax should be erroring properly%d,%d\n", l,
			numchar(buffer, '\"'));
		return (NULL);
	}
	if (numchar(buffer, '\"') != 0)
		buffer = loopquote(buffer, '\"');
	return (buffer);
}

// char	*envconcatlen(char *input, t_data *data)
// {
// 	int	i;
// 	int	len;

// 	i = 0;
// 	len = 0;
// 	while (input[i] != '\0')
// 	{
// 		/* code */
// 	}
// }

// char	*envconcat(char *input, t_data *data)
// {
// 	int	i;
// 	i = 0;
// 	while (input[i] != '\0')
// 	{
// 		if (input[i] == '$' && input[i + 1] != '\0')
// 		{

// 		}
// 	}
// }

char	*rmquote(char *input, t_data *data)
{
	// char *str;

	// str = envconcat(input, data);
	return (processstr(input, data));
}