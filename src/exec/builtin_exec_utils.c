/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 06:23:28 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 06:25:07 by odib             ###   ########.fr       */
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

int	is_builtin_command(const char *command)
{
	if (command == NULL)
		return (0);
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

int	execunset(t_command *command, t_data *data)
{
	if (!ft_strcmp(command->args->arg, "unset"))
	{
		data->cmd.status = unset_command(command->args, &data->env_list);
		return (data->cmd.status);
	}
	return (-1);
}

int	execexport(t_command *command, t_data *data)
{
	if (!ft_strcmp(command->args->arg, "export"))
	{
		data->cmd.status = export_command(&data->env_list, command->args);
		return (data->cmd.status);
	}
	return (-1);
}

int	execute_builtin_command_nofork(t_command *command, t_data *data)
{
	int	returnstatus;

	returnstatus = -1;
	dupexecnofork(command);
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
