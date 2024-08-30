/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:17:06 by odib              #+#    #+#             */
/*   Updated: 2024/08/31 06:18:53 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **tokens_to_args(char *tokens) {
    // Count the number of arguments
    int count = 0;
    char *temp = tokens;
    while (*temp) {
        while (*temp && *temp == ' ') temp++;
        if (*temp) count++;
        while (*temp && *temp != ' ') temp++;
    }

    // Allocate memory for the argument array (with an extra slot for NULL)
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args) {
        perror("malloc");
        return NULL;
    }

    // Tokenize the input string and fill the argument array
    int i = 0;
    char *token = strtok(tokens, " ");
    while (token) {
        args[i++] = strdup(token);  // Duplicate the token
        token = strtok(NULL, " ");
    }

    // NULL-terminate the argument array
    args[i] = NULL;

    return args;
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