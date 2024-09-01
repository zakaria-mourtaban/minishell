/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:09 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 17:59:57 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo_command(t_arg *args)
{
	int newline = 1;
	t_arg *current = args->next;

	if (current != NULL && strcmp(current->arg, "-n") == 0)
	{
		newline = 0;
		current = current->next;
	}

	while (current)
	{
		printf("%s", current->arg);
		if (current->next)
			printf(" ");

		current = current->next;
	}

	if (newline)
		printf("\n");
}