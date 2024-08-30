/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:12:30 by odib              #+#    #+#             */
/*   Updated: 2024/08/30 14:36:25 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    env_command(t_env *env_list)
{
    	t_env	*temp;

	temp = env_list;
	while (temp != NULL)
	{
		if (temp->hidden == 0)
		{
			/* code */
			printf("%s=\"%s\"\n", temp->key, temp->value);
		}
		temp = temp->next;
	}

}