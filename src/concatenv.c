/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:42:27 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/10 16:05:15 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*envvaluestr(char *key, t_data *data)
{
	t_env	*envtmp;
	t_value	*value;
	char	*valuestr;

	envtmp = data->env_list;
	while (envtmp != NULL && ft_strcmp(envtmp->key, key) != 0)
		envtmp = envtmp->next;
	if (envtmp == NULL)
		return (ft_strdup(""));
	value = envtmp->value_head;
	valuestr = ft_strdup("");
	while (value != NULL)
	{
		valuestr = ft_strjoingnl(valuestr, value->value);
		if (value->next != NULL)
			valuestr = ft_strjoingnl(valuestr, ":");
		value = value->next;
	}
	return (valuestr);
}

int	find_char_index(char *string, char c)
{
	int	i;

	i = 0;
	while (string[i] != c && string[i] != '\0')
		i++;
	return (i);
}

char	*concatenvloop(char *input, t_data *data)
{
	int		i;
	int		j;
	char	*before;
	char	*after;
	char	*key;

	i = 0;
	while (input[i] != '$' && input[i] != '\0')
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
		}
		i++;
	}
	if (input[i] == '\0' && i++)
		return (input);
	before = ft_strnew(i + 1);
	ft_strlcpy(before, input, i + 1);
	j = i;
	while (input[j] != ' ' && input[j] != '\0' && input[j] != '\"')
	{
		if (input[j] == '\'')
		{
			j++;
			while (input[j] != '\'' && input[j] != '\0')
				j++;
		}
		else
			j++;
	}
	if (input[j] == '\0')
		return (input);
	key = ft_strnew(j - i + 1);
	ft_strlcpy(key, (input + i), j - i + 1);
	after = ft_strnew(ft_strlen(input) - ft_strlen(before) + ft_strlen(key)
			+ 1);
	ft_strlcpy(after, (input + ft_strlen(before) + ft_strlen(key)),
		ft_strlen(input) - ft_strlen(before) + ft_strlen(key) + 1);
	free(input);
	input = ft_strdup("");
	input = ft_strjoingnl(input, before);
	if (ft_strlen(key) != 0)
	{
		input = ft_strjoingnl(input, "'");
		input = ft_strjoingnl(input, envvaluestr(key + 1, data));
		input = ft_strjoingnl(input, "'");
	}
	input = ft_strjoingnl(input, after);
	return (input);
}
// printf("before:%s\n", before);
// printf("key:%s\n", key);
// printf("after:%s\n", after);

int	dollarcount(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '$' && !isalpha(input[i + 1]))
		{
			i++;
			while (input[i] != ' ' && input[i] != '\0' && input[i] != '\"')
				i++;
			i++;
		}
		if (input[i] == '$')
			count++;
		if (input[i] == '\'')
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

char	*concatenv(char *input, t_data *data)
{
	char	*out;

	out = ft_strdup(input);
	while (dollarcount(out) != 0)
		out = concatenvloop(out, data);
	return (out);
}
