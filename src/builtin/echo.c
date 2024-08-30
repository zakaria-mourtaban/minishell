/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:09 by odib              #+#    #+#             */
/*   Updated: 2024/08/30 12:15:13 by odib             ###   ########.fr       */
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
void echo_command(char **args)
{
    bool newline = true;
    int i = 1;

    printf("test");
    while (args[i] && is_valid_n_flag(args[i]))
    {
        newline = false;
        i++;
    }
    
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");
}