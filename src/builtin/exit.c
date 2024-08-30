/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:54 by odib              #+#    #+#             */
/*   Updated: 2024/08/30 12:08:27 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void exit_command(char **args)
{
    int status = 0;

    if (args[1] != NULL)
        status = atoi(args[1]); 

    printf("Exiting with status %d\n", status);
    exit(status);
}