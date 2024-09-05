/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:38:08 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/04 12:26:54 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_cmdchain(t_tokens *cmdchain)
{
	t_tokens	*current;
	t_tokens	*next;

	if (cmdchain && cmdchain->previous)
	{
		free(cmdchain->previous->content);
		free(cmdchain->previous);
	}
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

void	free_arg_list(t_arg *head)
{
	t_arg	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp && tmp->arg != NULL)
			free(tmp->arg);
		if (tmp)
			free(tmp);
	}
}

void	free_command_list(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->args)
			free_arg_list(tmp->args);
		free(tmp);
	}
}
