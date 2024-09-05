/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 02:09:41 by odib              #+#    #+#             */
/*   Updated: 2024/09/05 02:12:07 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	contains_dot_or_slash(const char *str)
{
	while (*str)
	{
		if (*str == '.' || *str == '/')
		{
			return (1);
		}
		str++;
	}
	return (0);
}

void	printerrnocmd(t_tokens *token, t_data *data)
{
	data->cmd.status = 127;
	token->error = 1;
	printf("bash: %s: command not found\n", token->content);
}

void	printerrnodir(t_tokens *token, t_data *data)
{
	data->cmd.status = 126;
	token->error = 1;
	printf("bash: %s: is a directory\n", token->content);
}

void	printerrnofdir(t_tokens *token, t_data *data)
{
	data->cmd.status = 127;
	token->error = 1;
	printf("bash: %s: No such file or directory\n", token->content);
}
