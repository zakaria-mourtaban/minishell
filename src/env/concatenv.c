/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:42:27 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/03 12:26:21 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_char_index(char *string, char c)
{
	int	i;

	i = 0;
	while (string[i] != c && string[i] != '\0')
		i++;
	return (i);
}

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
	}
	free(key);
	return (valuestr);
}

char	*concatenv(char *input, t_data *data)
{
	char	*out;
	char	*temp_out;

	out = ft_strdup(input);
	while (dollarcount(out) != 0)
	{
		temp_out = out;
		out = concatenvloop(out, data);
		free(temp_out);
		printf("out:%s\n", out);
	}
	return (out);
}

// void	concatenvtoken(t_data *data)
// {
// 	t_tokens	*tmp;
//
// 	tmp = data->cmdchain;
// 	while (tmp)
// 	{
// 		tmp->content = handle_dollar_sign(tmp->content, data);
// 		tmp = tmp->next;
// 	}
// }
