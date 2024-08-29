/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:05:11 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/29 15:58:30 by zmourtab         ###   ########.fr       */
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
	if (!ft_strcmp(token->content, "/"))
		return (1);
	if (!ft_strcmp(token->content, "."))
		return (1);
	if (access(get_path(token->content, data->env_list), X_OK))
		return (1);
	printf("bash: %s: command not found\n", token->content);
	signalint = 127;
	return (0);
}

t_tokens	*nexttoken(t_tokens *tokens)
{
	while (tokens != NULL && tokens->id != TOKEN_PIPE)
		tokens = tokens->next;
	return (tokens);
}

t_tokens	*getnextspace(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	if (token == NULL)
		return (NULL);
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->next;
	return (tmp);
}

t_command *parse_tokens(t_tokens *tokens) {
    t_command *cmd_list = NULL;
    t_command *current_cmd = NULL;
    t_tokens *tmp = tokens;

    while (tmp) {
        // If current token is a word or command, handle it
        if (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND) {
            if (!current_cmd) {
                current_cmd = create_command_node();
                if (!current_cmd) {
                    fprintf(stderr, "Failed to create a new command node.\n");
                    return NULL;
                }
            }
            add_argument(current_cmd, tmp->content);
        }
        // Handle pipe tokens
        else if (tmp->id == TOKEN_PIPE) {
            if (current_cmd) {
                append_command_node(&cmd_list, current_cmd);
                current_cmd = NULL;
            }
        }
        // Handle file redirections
        else if (tmp->id == TOKEN_IN_FILE || tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE) {
            if (tmp->next && (tmp->next->id == TOKEN_WORD || tmp->next->id == TOKEN_COMMAND)) {
                if (!current_cmd) {
                    current_cmd = create_command_node();
                    if (!current_cmd) {
                        fprintf(stderr, "Failed to create a new command node.\n");
                        return NULL;
                    }
                }

                int flags = 0;
                if (tmp->id == TOKEN_IN_FILE) {
                    flags = O_RDONLY;
                } else if (tmp->id == TOKEN_OUT_FILE) {
                    flags = O_WRONLY | O_CREAT | O_TRUNC;
                    current_cmd->append = 0;
                } else if (tmp->id == TOKEN_OUT_A_FILE) {
                    flags = O_WRONLY | O_CREAT | O_APPEND;
                    current_cmd->append = 1;
                }

                int *file_desc = (tmp->id == TOKEN_IN_FILE) ? &current_cmd->infile : &current_cmd->outfile;
                *file_desc = open(tmp->next->content, flags, 0644);

                if (*file_desc < 0) {
                    perror("Failed to open file");
                } else {
                    const char *type = (tmp->id == TOKEN_IN_FILE) ? "input" : (tmp->id == TOKEN_OUT_FILE) ? "output" : "append output";
                    printf("Set %s file to: %s\n", type, tmp->next->content);
                }
                tmp = tmp->next; // Skip past the next token which is the file name
            }
        }

        // Move to the next token
        tmp = tmp->next;
    }

    // Append the last command node if it exists
    if (current_cmd) {
        append_command_node(&cmd_list, current_cmd);
    }
    
    return cmd_list;
}


void	free_arg_list(t_arg *head)
{
	t_arg	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->arg != NULL)
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
	t_command *cmd;
	t_arg *arg;
	int cmd_num;

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
	}
}