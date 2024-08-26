/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:05:11 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/26 13:50:53 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_arg	*create_arg_node(char *arg)
{
	t_arg	*new_arg;

	new_arg = (t_arg *)malloc(sizeof(t_arg));
	if (!new_arg)
		return (NULL);
	new_arg->arg = strdup(arg);
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
	if (access(get_path(token->content, data->env_list), X_OK))
		return (1);
	printf("bash: %s: command not found\n", token->content);
	return (0);
}

t_tokens	*nexttoken(t_tokens *tokens)
{
	while (tokens != NULL && tokens->id != TOKEN_PIPE)
		tokens = tokens->next;
	return (tokens);
}

t_command	*parse_tokens(t_tokens *tokens, t_data *data)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_tokens	*prev;

	prev = NULL;
	cmd_list = NULL;
	current_cmd = NULL;
	if (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND)
	{
		if (!current_cmd)
		{
			current_cmd = create_command_node();
			if (!current_cmd)
			{
				fprintf(stderr, "Failed to create a new command node.\n");
				return (NULL);
			}
		}
		add_argument(current_cmd, tokens->content);
	}
	prev = tokens;
	tokens = tokens->next;
	while (tokens)
	{
		if (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND)
		{
			if (!current_cmd)
			{
				current_cmd = create_command_node();
				if (!current_cmd)
				{
					fprintf(stderr, "Failed to create a new command node.\n");
					return (NULL);
				}
			}
			if ((prev->id == TOKEN_WORD || prev->id == TOKEN_COMMAND
					|| prev->id == TOKEN_SPACE))
				add_argument(current_cmd, tokens->content);
		}
		else if (tokens->id == TOKEN_PIPE)
		{
			if (current_cmd)
			{
				append_command_node(&cmd_list, current_cmd);
				current_cmd = NULL;
			}
		}
		else if (tokens->id == TOKEN_IN_FILE)
		{
			tokens = tokens->next;
			if (tokens->id == TOKEN_SPACE)
				tokens = tokens->next; // Move to the next token,
			if (tokens && (tokens->id == TOKEN_WORD
					|| tokens->id == TOKEN_COMMAND) && current_cmd)
			{
				current_cmd->infile = open(tokens->content, O_RDONLY);
				if (current_cmd->infile < 0)
					perror("Failed to open input file");
				else
					printf("Set input file to: %s\n", tokens->content);
			}
			tokens = nexttoken(tokens);
		}
		else if (tokens->id == TOKEN_OUT_FILE)
		{
			tokens = tokens->next;
			if (tokens->id == TOKEN_SPACE)
				tokens = tokens->next; // Move to the next token,
			if (tokens && (tokens->id == TOKEN_WORD
					|| tokens->id == TOKEN_COMMAND) && current_cmd)
			{
				current_cmd->outfile = open(tokens->content,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
				current_cmd->append = 0;
				if (current_cmd->outfile < 0)
					perror("Failed to open output file");
				else
					printf("Set output file to: %s\n", tokens->content);
			}
			tokens = nexttoken(tokens);
		}
		else if (tokens->id == TOKEN_OUT_A_FILE)
		{
			tokens = tokens->next;
			if (tokens->id == TOKEN_SPACE)
				tokens = tokens->next; // Move to the next token,
			if (tokens && (tokens->id == TOKEN_WORD
					|| tokens->id == TOKEN_COMMAND) && current_cmd)
			{
				current_cmd->outfile = open(tokens->content,
						O_WRONLY | O_CREAT | O_APPEND, 0644);
				current_cmd->append = 1;
				if (current_cmd->outfile < 0)
					perror("Failed to open append output file");
				else
					printf("Set append output file to: %s\n", tokens->content);
			}
			tokens = nexttoken(tokens);
		}
		if (tokens != NULL)
			tokens = tokens->next;
	}
	if (current_cmd)
		append_command_node(&cmd_list, current_cmd);
	(void)data;
	return (cmd_list);
}

void	free_arg_list(t_arg *head)
{
	t_arg	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->arg);
		free(tmp);
	}
}

void	free_command_list(t_command *head)
{
	t_command	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->args)
			free_arg_list(tmp->args);
		free(tmp);
	}
}

void	print_command_list(t_command *cmd_list)
{
	t_command	*cmd;
	t_arg		*arg;
	int			cmd_num;

	cmd = cmd_list;
	cmd_num = 1;
	while (cmd)
	{
		printf("Command %d:\n", cmd_num);
		arg = cmd->args;
		while (arg)
		{
			printf("  Arg: %s\n", arg->arg);
			arg = arg->next;
		}
		if (cmd->infile != STDIN_FILENO)
			printf("  Input File: (fd: %d)\n", cmd->infile);
		else
			printf("  Input File: STDIN\n");
		if (cmd->outfile != STDOUT_FILENO)
			printf("  Output File: (fd: %d)\n", cmd->outfile);
		else
			printf("  Output File: STDOUT\n");
		cmd = cmd->next;
		cmd_num++;
		printf("\n");
	}
}
