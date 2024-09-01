/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:54 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 11:34:11 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void exit_command(t_arg *args)
{
    int status = 0;
    t_arg *current = args->next;
    
    if (current->arg != NULL)
        status = atoi(current->arg); 

    printf("Exiting with status %d\n", status);
    exit(status);
}