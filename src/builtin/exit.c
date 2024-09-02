/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:54 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 00:05:09 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exit_command(t_arg *args)
{
	int		status;
	t_arg	*current;

	status = 0;
	current = args->next;
	if (current != NULL && current->arg != NULL)
	{
		status = atoi(current->arg);
		if (status == 0 && strcmp(current->arg, "0") != 0)
		{
			printf("Error: Invalid numeric argument '%s'\n", current->arg);
			perror("");
			return (1);
		}
	}
	printf("Exiting with status %d\n", status);
	exit(status);
}
