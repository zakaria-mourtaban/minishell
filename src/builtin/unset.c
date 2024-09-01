/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 11:37:39 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 11:40:45 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void unset(t_env **env_list, const char *key)
{
    t_env *current = *env_list;
    t_env *previous = NULL;

    while (current != NULL)
    {
        // Check if the current node's key matches the key to be unset
        if (strcmp(current->key, key) == 0)
        {
            if (previous == NULL) // The first node is the target
            {
                *env_list = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            // Free the memory allocated for the key, value, and the node itself
            free(current->key);
            free(current->value);
            free(current);

            return; // Key found and removed, exit the function
        }

        previous = current;
        current = current->next;
    }

    // If we reach here, the key was not found
    fprintf(stderr, "unset: %s: no such variable\n", key);
}

int unset_command(t_arg *args, t_env **env_list)
{
    t_arg *current = args->next; // Skipping the command itself

    if (current == NULL)
    {
        fprintf(stderr, "unset: not enough arguments\n");
        return 1;
    }

    while (current != NULL)
    {
        unset(env_list, current->arg);
        current = current->next;
    }

    return 0;
}