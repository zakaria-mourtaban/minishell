/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:42:27 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/31 23:09:26 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*envvaluestr(char *key, t_data *data)
{
	t_env	*envtmp;
	char	*value;
	char	*valuestr;

	printf("KEY:%s\n", key);
	if (ft_strlen(key) == 0)
		return (ft_strdup(""));
	envtmp = data->env_list;
	while (envtmp != NULL && ft_strcmp(envtmp->key, key) != 0)
	{
		printf("env:%s\n", envtmp->key);
		envtmp = envtmp->next;
	}
	if (envtmp == NULL || (ft_strlen(key) != ft_strlen(envtmp->key)))
		return (ft_strdup(""));
	value = envtmp->value;
	valuestr = ft_strdup("");
	if (value != NULL)
	{
		valuestr = ft_strjoingnl(valuestr, value);
		// if (value->next != NULL)
		// 	valuestr = ft_strjoingnl(valuestr, ":");
		// value = value->next;
	}
	free(key);
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
	int		inq;
	char	*before;
	char	*after;
	char	*key;

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
	before = ft_strnew(i + 1);
	ft_strlcpy(before, input, i + 1);
	j = i;
	while (input[j] != ' ' && input[j] != '\0' && input[j] != '\"'
		&& input[j] != '\'')
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
	printf("inq:%d\n", inq);
	key = ft_strnew(j - i + 1);
	ft_strlcpy(key, (input + i), j - i + 1);
	after = ft_strnew(ft_strlen(input) - ft_strlen(before) + ft_strlen(key)
			+ 1);
	ft_strlcpy(after, (input + j), ft_strlen(input) - ft_strlen(before)
		+ ft_strlen(key) + 1);
	free(input);
	key = envvaluestr(key + 1, data);
	input = ft_strdup("");
	input = ft_strjoingnl(input, before);
	input = ft_strjoingnl(input, key);
	input = ft_strjoingnl(input, after);
	free(before);
	free(key);
	free(after);
	return (input);
}
// printf("before:%s\n", before);
// printf("key:%s\n", key);
// printf("after:%s\n", after);

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
		if (input[i] == '$' && !isalpha(input[i + 1]))
		{
			i++;
			while (input[i] != ' ' && input[i] != '\0' && input[i] != '\"')
				i++;
			i++;
		}
		if (input[i] == '$')
			count++;
		if (input[i] == '"')
			inq = !inq;
		if (input[i] == '\'' && !inq)
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

char *concatenv(char *input, t_data *data) {
    char *out;

    out = ft_strdup(input);
    while (dollarcount(out) != 0) {
        char *temp_out = out;
        out = concatenvloop(out, data);
        
        // Free the previous 'out' to prevent memory leaks
        free(temp_out);
        
        printf("out:%s\n", out);
    }
    return (out);
}

void	concatenvtoken(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		
		tmp->content = handle_dollar_sign(tmp->content, data);
		tmp = tmp->next;
	}
	// free_cmdchain(tmp);
	// Free the cmdchain after processing all tokens
    //free_cmdchain(data->cmdchain);
    //data->cmdchain = NULL;  // Avoid dangling pointer
}
