/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:36:26 by odib              #+#    #+#             */
/*   Updated: 2024/09/04 20:45:07 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_before_part(const char *input, int *index)
{
	int		inq;
	int		i;

	i = 0;
	inq = 0;
	while (input[i] != '$' && input[i] != '\0')
	{
		if (input[i] == '"')
			inq = !inq;
		if (input[i] == '\'' && !inq)
		{
			i++;
			while (input[i] != '\'' && input[i] != '\0')
				i++;
		}
		i++;
	}
	*index = i;
	return (ft_strndup(input, i));
}

static char	*get_key_and_after(const char *input, int start_index)
{
	int		inq;
	int		j;

	inq = 0;
	j = start_index;
	while (input[j] != ' ' && input[j] != '\0'
		&& input[j] != '\"' && input[j] != '\'')
	{
		if (input[j] == '"')
			inq = !inq;
		if (input[j] == '\'' && !inq)
		{
			j++;
			while (input[j] != '\'' && input[j] != '\0')
				j++;
		}
		else
			j++;
	}
	return (ft_strdup(input + start_index));
}

static char	*replace_key_with_value(const char *before,
	char *key_and_after, t_data *data)
{
	char	*result;
	char	*env_value;
	char	*key;
	char	*after;

	key = ft_strndup(key_and_after, ft_strchr(key_and_after, ' ')
			- key_and_after);
	after = ft_strdup(ft_strchr(key_and_after, ' '));
	free(key_and_after);
	env_value = envvaluestr(key + 1, data);
	result = ft_strjoingnl(ft_strdup(""), before);
	result = ft_strjoingnl(result, env_value);
	result = ft_strjoingnl(result, after);
	free(env_value);
	free(key);
	free(after);
	return (result);
}

char	*concatenvloop(char *input, t_data *data)
{
	int		before_index;
	char	*before;
	char	*key_and_after;
	char	*new_input;

	before = get_before_part(input, &before_index);
	key_and_after = get_key_and_after(input, before_index);
	free(input);
	new_input = replace_key_with_value(before, key_and_after, data);
	free(before);
	return (new_input);
}

// char	*concatenvloop(char *input, t_data *data)
// {
// 	int		i;
// 	int		j;
// 	int		inq;
// 	char	*before;
// 	char	*after;
// 	char	*key;

// 	i = 0;
// 	inq = 0;
// 	while (input[i] != '$' && input[i] != '\0')
// 	{
// 		if (input[i] == '"')
// 			inq = !inq;
// 		if (input[i] == '\'' && !inq)
// 		{
// 			i++;
// 			while (input[i] != '\'' && input[i] != '\0')
// 				i++;
// 		}
// 		i++;
// 	}
// 	before = ft_strnew(i + 1);
// 	ft_strlcpy(before, input, i + 1);
// 	j = i;
// 	while (input[j] != ' ' && input[j] != '\0' && input[j] != '\"'
// 		&& input[j] != '\'')
// 	{
// 		if (input[j] == '"')
// 			inq = !inq;
// 		if (input[j] == '\'' && !inq)
// 		{
// 			j++;
// 			while (input[j] != '\'' && input[j] != '\0')
// 				j++;
// 		}
// 		else
// 			j++;
// 	}
// 	printf("inq:%d\n", inq);
// 	key = ft_strnew(j - i + 1);
// 	ft_strlcpy(key, (input + i), j - i + 1);
// 	after = ft_strnew(ft_strlen(input) - ft_strlen(before) + ft_strlen(key)
// 			+ 1);
// 	ft_strlcpy(after, (input + j), ft_strlen(input) - ft_strlen(before)
// 		+ ft_strlen(key) + 1);
// 	free(input);
// 	key = envvaluestr(key + 1, data);
// 	input = ft_strdup("");
// 	input = ft_strjoingnl(input, before);
// 	input = ft_strjoingnl(input, key);
// 	input = ft_strjoingnl(input, after);
// 	free(before);
// 	free(key);
// 	free(after);
// 	return (input);
// }