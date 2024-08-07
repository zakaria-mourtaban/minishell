/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:42:37 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/07 20:48:48 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	numchar(char *input, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] == c && input[i] != '\0')
		{
			count++;
			i++;
		}
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

char	*recursivequote(char *buffer, char c)
{
	int	ci;
	int	ic;

	while (numchar(buffer, c) != 0)
	{
		ic = 0;
		ci = 0;
		while (buffer[ic] != c && buffer[ic] != '\0')
			ic++;
		while (buffer[ic + 1] == c && buffer[ic + 1] != '\0')
			ic++;
		ci++;
		ci = ic + 1;
		while (buffer[ci + 1] != c && buffer[ci + 1] != '\0')
			ci++;
		ci++;
		buffer = removepair(buffer, ic, ci);
	}
	return (buffer);
}

char	*processstr(char *input, t_data *data)
{
	char	*buffer;
	int		l;

	l = 0;
	buffer = ft_strdup(input);
	if (numchar(buffer, '\'') % 2 != 0 && numchar(buffer, '\'') != 0)
		l++;
	if ((numchar(buffer, '\"') % 2 != 0 && numchar(buffer, '\"') != 0))
		l++;
	if (l != 0)
	{
		data->errorid = 2;
		printf("error syntax should be erroring properly\n");
		return (NULL);
	}
	if (numchar(buffer, '\"') != 0)
		buffer = recursivequote(buffer, '\"');
	return (buffer);
}

char	*tokenizer(char *input, t_data *data)
{
	return (processstr(input, data));
}