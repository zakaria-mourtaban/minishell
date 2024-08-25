/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 23:38:08 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/22 23:57:02 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_cmdchain(t_tokens *cmdchain)
{
    t_tokens *current;
    t_tokens *next;

    current = cmdchain;
    while (current != NULL)
    {
        next = current->next;
		if (current->content != NULL)
        	free(current->content);
		if (current != NULL)
        	free(current);
        current = next;
    }
}

void free_env_list(t_env *env_list)
{
    t_env *current;
    t_env *next;

    current = env_list;
    while (current != NULL)
    {
        next = current->next;
        // Free any dynamically allocated members of t_env
        // For example, if t_env has a 'name' and 'value' char pointer:
        // free(current->name);
        // free(current->value);
        free(current);
        current = next;
    }
}

void free_data(t_data *data)
{
    if (data != NULL)
    {
		if (data->cmdchain != NULL)
        	free_cmdchain(data->cmdchain);
		if (data->env_list != NULL)
        	free_env_list(data->env_list);
        // Free any other dynamically allocated members of t_data
        // For example:
        // free(data->cmd.command);
        // free(data->cmd.args);
        // etc.
    }
}