/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executepipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:40 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/29 16:21:44 by zmourtab         ###   ########.fr       */
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

#include <stdbool.h>
#include <string.h>

bool	is_builtin_command(const char *command)
{
	if (command == NULL)
		return (false);
	// Compare both length and content of the command
	switch (ft_strlen(command))
	{
	case 4:
		if (ft_ft_strcmp(command, "echo") == 0)
			return (true);
		if (ft_strcmp(command, "exit") == 0)
			return (true);
		break ;
	case 3:
		if (ft_strcmp(command, "pwd") == 0)
			return (true);
		if (ft_strcmp(command, "env") == 0)
			return (true);
		break ;
	case 6:
		if (ft_strcmp(command, "export") == 0)
			return (true);
		break ;
	case 5:
		if (ft_strcmp(command, "unset") == 0)
			return (true);
		break ;
	default:
		break ;
	}
	return (false);
}

void	execute_builtin_command(const char *command, char *argv[])
{
	if (command == NULL)
		return ;
	switch (ft_strlen(command))
	{
	case 4:
		if (ft_strcmp(command, "echo") == 0)
		{
			echo_command(argv);
			return ;
		}
		if (ft_strcmp(command, "exit") == 0)
		{
			exit_command(argv);
			return ;
		}
		break ;
	case 3:
		if (ft_strcmp(command, "pwd") == 0)
		{
			pwd_command(argv);
			return ;
		}
		if (ft_strcmp(command, "env") == 0)
		{
			env_command(argv);
			return ;
		}
		break ;
	case 5:
		if (ft_strcmp(command, "unset") == 0)
		{
			unset_command(argv);
			return ;
		}
		break ;
	case 6:
		if (ft_strcmp(command, "export") == 0)
		{
			export_command(argv);
			return ;
		}
		break ;
	default:
		break ;
	}
	fprintf(stderr, "Unknown command: %s\n", command);
}

void	execute_command(t_command *cmd, int *pipes, int i, int num_cmds,
		t_env *envp_lsit)
{
	char	**args;
	int		arg_count;
	t_arg	*current;
	pid_t	pid;
	char	*path;
	int		j;

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
		// printf("executing %s %d %d\n", cmd->args->arg, cmd->infile,
		// 	cmd->outfile);
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
		j = 0;
		while (current)
		{
			args[j] = current->arg;
			current = current->next;
			j++;
		}
		args[j] = NULL;
		// Execute the command
		if (args[0])
			path = get_path(args[0], envp_lsit);
		if (is_builtin_command(args[0]))
		{
			execute_builtin_command(args[0], args)
		}
		execve(path, args, environ);
		if (path != args[0])
			free(path);
		free(args);
		exit(1);
	}
}

void	execute_pipeline(t_command *cmds, t_data *data)
{
	int			*pipes;
	int			num_cmds;
	int			i;
	t_command	*current;
	int			sig;
	char		*path;

	sig = 0;
	signalint = 0;
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
		path = get_path(current->args->arg, data->env_list);
		execute_command(current, pipes, i, num_cmds, data->env_list);
		current = current->next;
		i++;
		free(path);
	}
	close_pipes(pipes, (num_cmds - 1) * 2);
	i = 0;
	while (i < num_cmds)
	{
		waitpid(-1, &sig, 0);
		i++;
	}
	if (signalint != 130)
		signalint = sig;
	free(pipes);
}
