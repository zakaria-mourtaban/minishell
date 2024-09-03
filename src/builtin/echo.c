/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:09 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 17:19:36 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_n_flag(const char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	echo_command(t_arg *args)
{
	int		newline;
	t_arg	*current;

	newline = 1;
	current = args->next;
	while (current != NULL && is_valid_n_flag(current->arg))
	{
		newline = 0;
		current = current->next;
	}
	while (current)
	{
		if (current->arg == NULL)
		{
			perror("Error: Null argument encountered.\n");
			return (1);
		}
		printf("%s", current->arg);
		if (current->next)
			printf(" ");
		current = current->next;
	}
	if (newline)
		printf("\n");
	return (0);
}
