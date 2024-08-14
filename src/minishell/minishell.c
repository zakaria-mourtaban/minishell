/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/15 12:41:39 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct s_arg
{
    char            *arg;
    struct s_arg    *next;
}                   t_arg;

typedef struct s_command
{
    t_arg           *args;
    int             infile;
    int             outfile;
    struct s_command *next;
}                   t_command;

t_arg *create_arg_node(char *arg)
{
    t_arg *new_arg;

    new_arg = (t_arg *)malloc(sizeof(t_arg));
    if (!new_arg)
        return (NULL);
    new_arg->arg = strdup(arg);
    new_arg->next = NULL;
    return (new_arg);
}

void add_argument(t_command *cmd, char *arg)
{
    t_arg *new_arg;
    t_arg *temp;

    if (!cmd || !arg)
        return;
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


t_command *create_command_node(void)
{
    t_command *new_cmd;

    new_cmd = (t_command *)malloc(sizeof(t_command));
    if (!new_cmd)
        return (NULL);
    new_cmd->args = NULL;
    new_cmd->infile = STDIN_FILENO;
    new_cmd->outfile = STDOUT_FILENO;
    new_cmd->next = NULL;
    return (new_cmd);
}
void append_command_node(t_command **cmd_list, t_command *new_cmd)
{
    t_command *temp;

    if (!cmd_list || !new_cmd)
        return;
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

t_command *parse_tokens(t_tokens *tokens)
{
    t_command *cmd_list = NULL;
    t_command *current_cmd = NULL;

    while (tokens)
    {
        printf("Processing token: id=%d, content=%s\n", tokens->id, tokens->content);

        if (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND)
        {    
            if (!current_cmd)
            {
                current_cmd = create_command_node();
                if (!current_cmd)
                {
                    fprintf(stderr, "Failed to create a new command node.\n");
                    return NULL;
                }
                printf("Created new command node.\n");
            }
            add_argument(current_cmd, tokens->content);
            printf("Added argument: %s\n", tokens->content);
        }
        else if (tokens->id == TOKEN_PIPE)
        {
            if (current_cmd)
            {
                append_command_node(&cmd_list, current_cmd);
                printf("Appended command to list. Starting new command.\n");
                current_cmd = NULL;
            }
        }
        else if (tokens->id == TOKEN_IN_FILE)
        {
            tokens = tokens->next; // Move to the next token, which should be the filename
            if (tokens && (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND) && current_cmd)
            {
                current_cmd->infile = open(tokens->content, O_RDONLY);
                if (current_cmd->infile < 0)
                    perror("Failed to open input file");
                else
                    printf("Set input file to: %s\n", tokens->content);
            }
        }
        else if (tokens->id == TOKEN_OUT_FILE)
        {
            tokens = tokens->next; // Move to the next token, which should be the filename
            if (tokens && (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND) && current_cmd)
            {
                current_cmd->outfile = open(tokens->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (current_cmd->outfile < 0)
                    perror("Failed to open output file");
                else
                    printf("Set output file to: %s\n", tokens->content);
            }
        }
        else if (tokens->id == TOKEN_OUT_A_FILE)
        {
            tokens = tokens->next; // Move to the next token, which should be the filename
            if (tokens && (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND) && current_cmd)
            {
                current_cmd->outfile = open(tokens->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (current_cmd->outfile < 0)
                    perror("Failed to open append output file");
                else
                    printf("Set append output file to: %s\n", tokens->content);
            }
        }
        tokens = tokens->next;
    }

    if (current_cmd)
    {
        append_command_node(&cmd_list, current_cmd);
        printf("Appended final command to list.\n");
    }

    printf("Parsing complete\n");
    return cmd_list;
}


void free_arg_list(t_arg *head)
{
    t_arg *tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->arg);
        free(tmp);
    }
}


void free_command_list(t_command *head)
{
    t_command *tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        if (tmp->args)
            free_arg_list(tmp->args);
        free(tmp);
    }
}

void print_command_list(t_command *cmd_list)
{
    t_command *cmd = cmd_list;
    t_arg *arg;
    int cmd_num = 1;

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


volatile int	signalint;
int	main(int ac, char **av, char **env)
{
	char		*input;
	t_data		data;
	t_command	*commands;
	
	data.cmd.running = 0;
	art();
	using_history();
	data.env_list = NULL;
	init_copy_envp(&(data.env_list), env);

	// Initial test input to tokenize, parse, and run
	//tokenizer(concatenv(ft_strdup("test $ $HOME \"$HOME $HOME\" '$HOME' test | < >> << >"), &data), &data);
	//remove_quotes(data.cmdchain);
	//printcmds(&data);

	// Parse the tokens into command structures
	
	
	
	// You can now print or execute commands here for debugging

	while (1)
	{
		interactivemode(&data, &input);
		if (input == NULL)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(input) != 0)
			add_history(input);

		// Non-interactive mode processing
		noninteractivemode(&data, &input);

		// Re-tokenize and re-parse each new input
		tokenizer(concatenv(input, &data), &data);
		remove_quotes(data.cmdchain);
		commands = parse_tokens(data.cmdchain);
		print_command_list(commands);
		

		// Execute parsed commands (you'll need an execution function for this)
		// execute_commands(commands, env, &data);

		// Free parsed commands
		free_command_list(commands);

		// free(input); // Uncomment this line to free the input if needed
	}
	// Clean up before exiting
	free_data(&data);
	(void)ac;
	(void)av;
	return (0);
}
