/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorcheck.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 04:33:02 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 04:34:32 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_path(t_tokens *token, t_data *data)
{
	struct stat	statbuf;
	char		*str;

	if (is_builtin_command(token->content))
		return (0);
	str = get_path((char *)token->content, data->env_list);
	if (access(str, X_OK) != 0 && !contains_dot_or_slash(token->content)
		&& !is_builtin_command(str))
	{
		printerrnocmd(token, data);
		return (free(str), 1);
	}
	else if (stat(token->content, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)
		&& !is_builtin_command(str))
	{
		printerrnodir(token, data);
		return (free(str), 1);
	}
	else if (access(str, X_OK) != 0)
	{
		printerrnofdir(token, data);
		return (free(str), 1);
	}
	free(str);
	return (0);
}

t_tokens	*getnextcommand(t_tokens *tmp)
{
	while (tmp && tmp->id != TOKEN_PIPE)
		tmp = tmp->next;
	return (tmp);
}

void	handleerr(t_data *data, t_tokens **tmp, t_tokens **tmpprint)
{
	data->cmd.status = 2;
	printf("%d\n", (*tmp)->error);
	printerror(*tmpprint);
	(*tmp)->error = 1;
	*tmp = getnextcommand(*tmp);
}

void	handleerrpipe(t_data *data, t_tokens **tmp, t_tokens **tmpprint)
{
	t_tokens	*tmptk;

	tmptk = data->cmdchain;
	data->cmd.status = 2;
	printf("bash: syntax error near unexpected token `|'\n");
	while (tmptk)
	{
		tmptk->error = 1;
		tmptk = tmptk->next;
	}
	(void)tmpprint;
	(void)tmp;
}
	// *tmp = getnextcommand(*tmp);
	// if ((*tmp) && (*tmp)->next)
	// 	(*tmp) = (*tmp)->next->next;

int	checksyntaxerror(t_data *data)
{
	t_tokens	*tmp;

	checkpipeerr(data);
	tmp = data->cmdchain;
	while (tmp)
	{
		if (casenext(&tmp))
			continue ;
		if (tmp->id == TOKEN_IN_FILE && checkfilein(tmp))
			handleerr(data, &tmp, &tmp);
		else if ((tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE)
			&& checkfileout(tmp))
			handleerr(data, &tmp, &(tmp->next));
		else if (tmp->id == TOKEN_COMMAND && check_path(tmp, data))
		{
			tmp->error = 1;
			tmp = getnextcommand(tmp);
			continue ;
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (0);
}
