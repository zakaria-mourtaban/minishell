/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 05:10:31 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 05:12:26 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tokens	*newnode(char *data, int type)
{
	t_tokens	*ptr;

	ptr = malloc(sizeof(t_tokens));
	if (ptr == NULL)
		return (NULL);
	ptr->content = ft_strdup(data);
	if (ptr->content == NULL)
	{
		free(ptr);
		return (NULL);
	}
	ptr->id = type;
	ptr->next = NULL;
	ptr->previous = NULL;
	ptr->error = 0;
	return (ptr);
}

void	addnode(t_tokens **tmp, char *data, int type)
{
	t_tokens	*new_node;

	new_node = newnode(data, type);
	new_node->error = 0;
	(*tmp)->next = new_node;
	new_node->previous = (*tmp);
}
