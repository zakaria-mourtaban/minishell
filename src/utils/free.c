/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:38:08 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 18:00:29 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_cmdchain(t_tokens *cmdchain)
{
	t_tokens	*current;
	t_tokens	*next;

	free(cmdchain->previous->content);
	free(cmdchain->previous);
	current = cmdchain;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current != NULL)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		if (current != NULL)
			free(current);
		current = next;
	}
}

void	free_data(t_data *data)
{
	if (data != NULL)
	{
		if (data->cmdchain != NULL)
			free_cmdchain(data->cmdchain);
		free_env_list(data->env_list);
	}
}