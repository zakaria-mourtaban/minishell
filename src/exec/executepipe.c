/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executepipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:40 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/03 21:16:36 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	is_builtin_command(const char *command)
{
	if (command == NULL)
		return (0);
	// Check command length and content
	if (ft_strlen(command) == 2 && ft_strcmp(command, "cd") == 0)
		return (1);
	else if (ft_strlen(command) == 4 && (ft_strcmp(command, "echo") == 0
			|| ft_strcmp(command, "exit") == 0))
		return (1);
	else if (ft_strlen(command) == 3 && (ft_strcmp(command, "pwd") == 0
			|| ft_strcmp(command, "env") == 0))
		return (1);
	else if (ft_strlen(command) == 6 && ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strlen(command) == 5 && ft_strcmp(command, "unset") == 0)
		return (1);
	else
		return (0);
}

int	execute_builtin_command(t_command *command, t_data *data)
{
	if (command == NULL)
		return (-1);
	if (ft_strcmp(command->args->arg, "echo") == 0
		&& ft_strlen(command->args->arg) == 4)
	{
		data->cmd.status = echo_command(command->args);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "cd") == 0
		&& ft_strlen(command->args->arg) == 2)
	{
		data->cmd.status = change_dir(command->args, data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "exit") == 0
		&& ft_strlen(command->args->arg) == 4)
	{
		exit_command(command->args);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "unset") == 0
		&& ft_strlen(command->args->arg) == 5)
	{
		data->cmd.status = unset_command(command->args, &data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "pwd") == 0
		&& ft_strlen(command->args->arg) == 3)
	{
		data->cmd.status = pwd_command();
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "env") == 0
		&& ft_strlen(command->args->arg) == 3)
	{
		data->cmd.status = env_command(data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "export") == 0
		&& ft_strlen(command->args->arg) == 6)
	{
		printf("%s\n", command->args->arg);
		data->cmd.status = export_command(&data->env_list, command->args);
		return (data->cmd.status);
	}
	// Uncomment and implement if needed
	// if (strcmp(command, "unset") == 0) {
	//     unset_command(argv);
	//     return ;
	// }
	// fprintf(stderr, "Unknown command: %s\n", command);
	return (-1);
}

int	execute_builtin_command_nofork(t_command *command, t_data *data)
{
	if (command == NULL)
		return (-1);
	if (command->outfile != -1)
	{
		dup2(command->outfile, STDOUT_FILENO);
	}
	// Handle input redirection if infile is provided
	if (command->infile != -1)
	{
		dup2(command->infile, STDIN_FILENO);
	}
	if (ft_strcmp(command->args->arg, "echo") == 0
		&& ft_strlen(command->args->arg) == 4)
	{
		data->cmd.status = echo_command(command->args);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "cd") == 0
		&& ft_strlen(command->args->arg) == 2)
	{
		data->cmd.status = change_dir(command->args, data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "exit") == 0
		&& ft_strlen(command->args->arg) == 4)
	{
		exit_command(command->args);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "unset") == 0
		&& ft_strlen(command->args->arg) == 5)
	{
		data->cmd.status = unset_command(command->args, &data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "pwd") == 0
		&& ft_strlen(command->args->arg) == 3)
	{
		data->cmd.status = pwd_command();
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "env") == 0
		&& ft_strlen(command->args->arg) == 3)
	{
		data->cmd.status = env_command(data->env_list);
		return (data->cmd.status);
	}
	if (ft_strcmp(command->args->arg, "export") == 0
		&& ft_strlen(command->args->arg) == 6)
	{
		data->cmd.status = export_command(&data->env_list, command->args);
		return (data->cmd.status);
	}
	// Uncomment and implement if needed
	// if (strcmp(command, "unset") == 0) {
	//     unset_command(argv);
	//     return ;
	// }
	// fprintf(stderr, "Unknown command: %s\n", command);
	return (-1);
}

pid_t	execute_command(t_command *cmd, int *pipes, int i, int num_cmds,
		t_data *data)
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
		// printf("executing %s %d %d\n", cmd->args->arg, cmd->infile,
		for (int j = 0; j < 2 * (num_cmds - 1); j++)
		{
			close(pipes[j]);
		}
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
			path = get_path(args[0], data->env_list);
		if (is_builtin_command(args[0]))
			execute_builtin_command(cmd, data);
		else if (cmd->error == 0)
			execve(path, args, data->env);
		else
		{
			if (path != args[0])
				free(path);
			free(args);
			exit(127);
		}
		if (path != args[0])
			free(path);
		free(args);
		exit(data->cmd.status);
	}
	return (pid);
}

void	appendpid(pid_t pid, t_pids *pids)
{
	while (pids != NULL)
		pids = pids->next;
	pids = malloc(sizeof(t_pids));
	pids->pid = pid;
	pids->next = NULL;
}

void	execute_pipeline(t_command *cmds, t_data *data)
{
	int			*pipes;
	int			num_cmds;
	int			i;
	pid_t		pid;
	t_command	*current;
	int			in;
	int			out;
	char		*path;

	num_cmds = 0;
	pipes = NULL;
	current = cmds;
	while (current)
	{
		if (access(current->args->arg, X_OK))
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
		if (ft_strlen(current->args->arg) == 0)
		{
			current = current->next;
			continue ;
		}
		path = get_path(current->args->arg, data->env_list);
		if (is_builtin_command(current->args->arg) && current->next == NULL
			&& i == 0 && current->error == 0)
		{
			in = dup(STDIN_FILENO);
			out = dup(STDOUT_FILENO);
			execute_builtin_command_nofork(current, data);
			dup2(in, STDIN_FILENO);
			dup2(out, STDOUT_FILENO);
			close(in);
			close(out);
		}
		else if (current->error == 0)
			pid = execute_command(current, pipes, i, num_cmds, data);
		if (current->infile > 1)
			close(current->infile);
		if (current->outfile > 1)
			close(current->outfile);
		current = current->next;
		i++;
		free(path);
	}
	close_pipes(pipes, (num_cmds - 1) * 2);
	i = 0;
	while (i < num_cmds - 1)
	{
		wait(&data->cmd.status);
		i++;
	}
	waitpid(pid,&data->cmd.status,0);
	if (signalint == 130)
		data->cmd.status = 130;
	if (pipes != NULL)
		free(pipes);
}
