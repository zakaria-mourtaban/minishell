/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executepipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:40 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/06 06:30:28 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ex2(t_ex *ex)
{
	if (ex->i > 0)
		dup2(ex->pipes[(ex->i - 1) * 2], STDIN_FILENO);
	if (ex->i < ex->num_cmds - 1)
		dup2(ex->pipes[ex->i * 2 + 1], STDOUT_FILENO);
	else if (ex->cmd->outfile != -1)
		dup2(ex->cmd->outfile, STDOUT_FILENO);
	if (ex->cmd->infile != -1)
		dup2(ex->cmd->infile, STDIN_FILENO);
	ex->j = 0;
	while (ex->j < 2 * (ex->num_cmds - 1))
	{
		close(ex->pipes[ex->j]);
		ex->j++;
	}
}

void	ex3(t_ex *ex)
{
	ex->arg_count = 0;
	ex->current = ex->cmd->args;
	while (ex->current)
	{
		ex->arg_count++;
		ex->current = ex->current->next;
	}
	ex->args = malloc(sizeof(char *) * (ex->arg_count + 1));
	if (!ex->args)
	{
		perror("malloc");
		exit(1);
	}
	ex->current = ex->cmd->args;
	ex->j = 0;
	while (ex->current)
	{
		ex->args[ex->j] = ex->current->arg;
		ex->current = ex->current->next;
		ex->j++;
	}
	ex->args[ex->j] = NULL;
	if (ex->args[0])
		ex->path = get_path(ex->args[0], ex->data->env_list);
}

void	ex4(t_ex *ex)
{
	if (is_builtin_command(ex->args[0]) && ex->cmd)
		execute_builtin_command(ex->cmd, ex->data);
	else if (ex->cmd->error == 0)
		execve(ex->path, ex->args, ex->data->env);
	else
	{
		if (ex->path != ex->args[0])
			free(ex->path);
		free(ex->args);
		exit(127);
	}
	if (ex->path != ex->args[0])
	{
		free(ex->path);
		free(ex->args);
		exit(ex->data->cmd.status);
	}
}

void	execute_command(t_ex *ex)
{
	ex1(&ex->pid);
	if (ex->pid == 0)
	{
		ex2(ex);
		ex3(ex);
		ex4(ex);
	}
}
