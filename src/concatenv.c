/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:42:27 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/09 17:02:13 by zmourtab         ###   ########.fr       */
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
	while (input[j] != ' ' && input[j] != '\0')
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
	if (input[i] == '$' && input[i + 1] != ' ' && input[i + 1] != '\0')
	{
		key = ft_strnew(j - i + 1);
		ft_strlcpy(key, (input + i), j - i + 1);
	}
	else
		key = ft_strdup("$");
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
