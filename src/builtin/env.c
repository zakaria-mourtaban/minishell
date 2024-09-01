/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:12:30 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 17:59:58 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	env_command(t_env *env_list)
{
	t_env *temp;

	temp = env_list;
	while (temp != NULL)
	{
		if (temp->hidden == 0)
		{
			printf("%s=\"%s\"\n", temp->key, temp->value);
		}
		temp = temp->next;
	}
}