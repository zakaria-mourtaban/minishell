/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:54 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 18:35:49 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int exit_command(t_arg *args)
 {
    int status = 0;
    t_arg *current = args->next;

    if (current != NULL && current->arg != NULL) {
        status = atoi(current->arg);
        if (status == 0 && strcmp(current->arg, "0") != 0) {
            fprintf(stderr, "Error: Invalid numeric argument '%s'\n", current->arg);
            return 1; // Return 1 for invalid argument
        }
    }

    printf("Exiting with status %d\n", status);
    exit(status);
}