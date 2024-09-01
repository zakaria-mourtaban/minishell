/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:09 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 18:30:00 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_valid_n_flag(const char *arg)
{
    if (arg[0] != '-' || arg[1] != 'n')
        return false;

    for (int i = 1; arg[i] != '\0'; i++)
    {
        if (arg[i] != 'n')
            return false;
    }

    return true;
}
int echo_command(t_arg *args) 
{
    int newline = 1;  
    t_arg *current = args->next;


    while (current != NULL && is_valid_n_flag(current->arg)) {
        newline = 0; 
        current = current->next;
    }
    while (current) {
        if (current->arg == NULL) {
            fprintf(stderr, "Error: Null argument encountered.\n");
            return 1; 
        }
        printf("%s", current->arg);
        if (current->next) {
            printf(" ");
        }
        current = current->next;
    }
    if (newline) {
        printf("\n");
    }

    return 0;
}