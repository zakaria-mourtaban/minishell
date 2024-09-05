/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 02:36:25 by odib              #+#    #+#             */
/*   Updated: 2024/09/04 20:58:14 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expp1(t_exp *xp, t_command *cmds, t_data *data)
{
	xp->num_cmds = 0;
	xp->pipes = NULL;
	xp->current = cmds;
	while (xp->current)
	{
		if (access(xp->current->args->arg, X_OK))
			xp->num_cmds++;
		xp->current = xp->current->next;
	}
	if (xp->num_cmds == 0)
		xp->num_cmds++;
	xp->pipes = malloc(sizeof(int) * (xp->num_cmds - 1) * 2);
	(void)data;
}

void	expp2(t_exp *xp, t_data *data)
{
	t_ex	ex;

	ex.pipes = xp->pipes;
	ex.i = xp->i;
	ex.num_cmds = xp->num_cmds;
	ex.data = data;
	ex.cmd = xp->current;
	xp->path = get_path(xp->current->args->arg, data->env_list);
	if (is_builtin_command(xp->current->args->arg) && xp->current->next == NULL
		&& xp->i == 0 && xp->current->error == 0)
	{
		xp->in = dup(STDIN_FILENO);
		xp->out = dup(STDOUT_FILENO);
		execute_builtin_command_nofork(xp->current, data);
		dup2(xp->in, STDIN_FILENO);
		dup2(xp->out, STDOUT_FILENO);
		close(xp->in);
		close(xp->out);
	}
	else if (xp->current->error == 0)
		execute_command(&ex);
}

void	expp3(t_exp *xp, t_data *data)
{
	if (xp->current->infile > 1)
		close(xp->current->infile);
	if (xp->current->outfile > 1)
		close(xp->current->outfile);
	xp->current = xp->current->next;
	xp->i++;
	free(xp->path);
	(void)data;
}

void	expp4(t_exp *xp, t_data *data)
{
	close_pipes(xp->pipes, (xp->num_cmds - 1) * 2);
	xp->i = 0;
	while (xp->i < xp->num_cmds)
	{
		wait(&data->cmd.status);
		xp->i++;
	}
	if (g_signalint == 130)
		data->cmd.status = 130;
	if (xp->pipes != NULL)
		free(xp->pipes);
}

int	expp5(t_exp *xp, t_data *data)
{
	while (xp->i < xp->num_cmds - 1)
	{
		if (pipe(xp->pipes + xp->i * 2) == -1)
		{
			perror("pipe");
			free(xp->pipes);
			return (1);
		}
		xp->i++;
	}
	return (0);
	(void)data;
}
