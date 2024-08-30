/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:17:06 by odib              #+#    #+#             */
/*   Updated: 2024/08/30 14:18:20 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **tokens_to_args(t_tokens *tokens)
{
    int count;
    t_tokens *temp;
    char **args;

    count = 0;
    temp = tokens;
    // Count the number of TOKEN_WORD tokens
    while (temp)
    {
        if (temp->id == TOKEN_WORD)
            count++;
        temp = temp->next;
    }

    // Allocate space for args array (NULL-terminated)
    args = malloc((count + 1) * sizeof(char *));
    if (!args)
        return NULL;

    // Populate the args array with TOKEN_WORD values
    int i = 0;
    while (tokens)
    {
        if (tokens->id == TOKEN_WORD)
            args[i++] = strdup(tokens->content); // Duplicate the token value
        tokens = tokens->next;
    }
    args[i] = NULL; // Null-terminate the array

    return (args);
}

void free_args(char **args)
{
    int i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}