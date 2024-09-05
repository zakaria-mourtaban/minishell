/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 06:29:02 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 06:30:54 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec3(t_command *command, t_data *data)
{
	if (!ft_strcmp(command->args->arg, "env"))
		return (execenv(command, data));
	if (!ft_strcmp(command->args->arg, "pwd"))
		return (execpwd(command, data));
	return (-1);
}

int	exec4(t_command *command, t_data *data)
{
	if (!ft_strcmp(command->args->arg, "echo"))
		return (exececho(command, data));
	if (!ft_strcmp(command->args->arg, "exit"))
		return (execexit(command, data));
	return (-1);
}

int	execute_builtin_command(t_command *command, t_data *data)
{
	int	returnstatus;

	returnstatus = -1;
	if (ft_strlen(command->args->arg) == 2)
		returnstatus = (execcd(command, data));
	if (ft_strlen(command->args->arg) == 3)
		returnstatus = (exec3(command, data));
	if (ft_strlen(command->args->arg) == 4)
		returnstatus = (exec4(command, data));
	if (ft_strlen(command->args->arg) == 5)
		returnstatus = (execunset(command, data));
	if (ft_strlen(command->args->arg) == 6)
		returnstatus = (execexport(command, data));
	return (returnstatus);
}

// if (ft_strcmp(command->args->arg, "cd") == 0
// 	&& ft_strlen(command->args->arg) == 2)
// if (ft_strcmp(command->args->arg, "pwd") == 0
// 	&& ft_strlen(command->args->arg) == 3)
// if (ft_strcmp(command->args->arg, "env") == 0
// 	&& ft_strlen(command->args->arg) == 3)
// if (ft_strcmp(command->args->arg, "echo") == 0
// 	&& ft_strlen(command->args->arg) == 4)
// if (ft_strcmp(command->args->arg, "exit") == 0
// 	&& ft_strlen(command->args->arg) == 4)
// if (ft_strcmp(command->args->arg, "unset") == 0
// 	&& ft_strlen(command->args->arg) == 5)
// if (ft_strcmp(command->args->arg, "export") == 0
// 	&& ft_strlen(command->args->arg) == 6)

void	dupexecnofork(t_command *command)
{
	if (command->outfile != -1)
		dup2(command->outfile, STDOUT_FILENO);
	if (command->infile != -1)
		dup2(command->infile, STDIN_FILENO);
}

void	ex1(pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		perror("fork");
		exit(1);
	}
}
