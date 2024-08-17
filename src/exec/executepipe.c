/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executepipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:40 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/17 21:43:24 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char	**environ;

void	close_pipes(int *pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i]);
		i++;
	}
}

void	execute_command(t_command *cmd, int *pipes, int i, int num_cmds)
{
	char	**args;
	int		arg_count;
	t_arg	*current;
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		// Handle pipe input
		if (i > 0)
		{
			dup2(pipes[(i - 1) * 2], STDIN_FILENO);
		}

		// Handle pipe output
		if (i < num_cmds - 1)
		{
			dup2(pipes[i * 2 + 1], STDOUT_FILENO);
		}
		// Handle output redirection if outfile is provided
		else if (cmd->outfile != -1)
		{
			dup2(cmd->outfile, STDOUT_FILENO);
		}

		// Handle input redirection if infile is provided
		if (cmd->infile != -1)
		{
			dup2(cmd->infile, STDIN_FILENO);
		}
		
		// Close all pipe ends in child process
		for (int j = 0; j < 2 * (num_cmds - 1); j++)
		{
			close(pipes[j]);
		}

		// Construct the arguments array
		arg_count = 0;
		current = cmd->args;
		while (current)
		{
			arg_count++;
			current = current->next;
		}
		args = malloc(sizeof(char *) * (arg_count + 1));
		if (!args)
		{
			perror("malloc");
			exit(1);
		}
		current = cmd->args;
		int j = 0;
		while (current)
		{
			args[j] = current->arg;
			current = current->next;
			j++;
		}
		args[j] = NULL;

		// Execute the command
		path = get_path(args[0], environ);
		execve(path, args, environ);
		perror("execve");
		free(args);
		if (path != args[0])
			free(path);
		exit(1);
	}
}

void	execute_pipeline(t_command *cmds)
{
	int			*pipes;
	int			num_cmds;
	int			i;
	t_command	*current;

	num_cmds = 0;
	current = cmds;
	while (current)
	{
		num_cmds++;
		current = current->next;
	}
	pipes = malloc(sizeof(int) * (num_cmds - 1) * 2);
	if (!pipes)
		return ;
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes + i * 2) == -1)
		{
			perror("pipe");
			free(pipes);
			return ;
		}
		i++;
	}
	i = 0;
	current = cmds;
	while (current)
	{
		execute_command(current, pipes, i, num_cmds);
		current = current->next;
		i++;
	}
	close_pipes(pipes, (num_cmds - 1) * 2);
	i = 0;
	while (i < num_cmds)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	free(pipes);
}

