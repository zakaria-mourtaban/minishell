/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 06:20:30 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 06:31:09 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exececho(t_command *command, t_data *data)
{
	data->cmd.status = echo_command(command->args);
	return (data->cmd.status);
	return (-1);
}

int	execcd(t_command *command, t_data *data)
{
	if (!ft_strcmp(command->args->arg, "cd"))
	{
		data->cmd.status = change_dir(command->args, data->env_list);
		return (data->cmd.status);
	}
	return (-1);
}

int	execpwd(t_command *command, t_data *data)
{
	data->cmd.status = pwd_command();
	return (data->cmd.status);
	(void)command;
}

int	execenv(t_command *command, t_data *data)
{
	data->cmd.status = env_command(data->env_list);
	return (data->cmd.status);
	(void)command;
}

int	execexit(t_command *command, t_data *data)
{
	data->cmd.status = exit_command(command->args);
	return (data->cmd.status);
}
