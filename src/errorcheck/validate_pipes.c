/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 04:34:04 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 04:35:39 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	casenext(t_tokens **tmp)
{
	if ((*tmp)->error == 1)
	{
		(*tmp) = getnextcommand((*tmp));
		if (*tmp)
			(*tmp) = (*tmp)->next;
		return (1);
	}
	return (0);
}

void	checkpipeerr(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		if (casenext(&tmp))
			continue ;
		if (tmp->id == TOKEN_PIPE && checkpipe(tmp))
			handleerrpipe(data, &tmp, &tmp);
		tmp = tmp->next;
	}
}
