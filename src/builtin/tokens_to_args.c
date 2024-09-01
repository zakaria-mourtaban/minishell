/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:17:06 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 18:00:03 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**tokens_to_args(char *tokens)
{
	int		count;
	char	*temp;
	char	**args;
	int		i;
	char	*token;

	count = 0;
	temp = tokens;
	while (*temp)
	{
		while (*temp && *temp == ' ')
			temp++;
		if (*temp)
			count++;
		while (*temp && *temp != ' ')
			temp++;
	}
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	token = strtok(tokens, " ");
	while (token)
	{
		args[i++] = strdup(token);
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}
void	free_args(char **args)
{
	int i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}