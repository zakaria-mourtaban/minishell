/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 11:37:39 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 00:23:04 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*previous;

	current = *env_list;
	previous = NULL;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				*env_list = current->next;
			else
				previous->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
	printf("unset: %s: no such variable", key);
	perror("\n");
}

int	unset_command(t_arg *args, t_env **env_list)
{
	t_arg	*current;

	current = args->next;
	if (current == NULL)
	{
		perror("unset: not enough arguments\n");
		return (1);
	}
	while (current != NULL)
	{
		unset(env_list, current->arg);
		current = current->next;
	}
	return (0);
}
