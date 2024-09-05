/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:55:39 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 07:03:24 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_arg	*create_arg_node(char *arg)
{
	t_arg	*new_arg;

	new_arg = (t_arg *)malloc(sizeof(t_arg));
	if (!new_arg)
		return (NULL);
	new_arg->arg = ft_strdup(arg);
	new_arg->next = NULL;
	return (new_arg);
}

void	add_argument(t_command *cmd, char *arg)
{
	t_arg	*new_arg;
	t_arg	*temp;

	if (!cmd || !arg)
		return ;
	new_arg = create_arg_node(arg);
	if (!cmd->args)
	{
		cmd->args = new_arg;
	}
	else
	{
		temp = cmd->args;
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
}

t_command	*create_command_node(void)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->infile = STDIN_FILENO;
	new_cmd->outfile = STDOUT_FILENO;
	new_cmd->next = NULL;
	new_cmd->error = 0;
	return (new_cmd);
}

void	append_command_node(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*temp;

	if (!cmd_list || !new_cmd)
		return ;
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
	}
	else
	{
		temp = *cmd_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_cmd;
	}
}

int	hasaccess(t_tokens *token, t_data *data)
{
	if (!ft_strcmp(token->content, "/"))
		return (1);
	if (!ft_strcmp(token->content, "."))
		return (1);
	if (access(get_path(token->content, data->env_list), X_OK))
		return (1);
	printf("bash: %s: commandsda not found\n", token->content);
	data->cmd.status = 127;
	return (0);
}
