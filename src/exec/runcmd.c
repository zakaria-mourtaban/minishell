

#include "../../includes/minishell.h"


// runs cmd without any handling
void	runcmd(const char *input, char **env, t_data *data)
{
	pid_t pid;
	int status;

	data->cmd.running = 1;
	data->cmd.cmd = ft_split(input, ' ');
	pid = fork();
	if (pid == -1)
		printf("error in fork should be doing something here");
	if (pid == 0)
	{
		status = execve(get_path(data->cmd.cmd[0], data->env_list),
				data->cmd.cmd, env);
		if (status != 0)
		{
			signalint = 127;
			printf("%s: command not found\n", data->cmd.cmd[0]);
			exit(status);
		}
		exit(status);
	}
	else
		data->cmd.pid = pid;
}
// handles cmd and executes,
// can be modified for future implementation of signals

// void	parsecmd(const char *input, t_data *data)
// {
// 	char *tokens;
// 	tokens = rmquote((char *)input, data);
// }cd , echo , unset , exit

void	free_tokens(t_tokens *head)
{
	t_tokens *current;
	t_tokens *next;

	current = head;
	while (current != NULL)
	{
		next = current->next;   // Save the next node
		free(current->content); // Free the content of the current node
		free(current);          // Free the current node
		current = next;         // Move to the next node
	}
}

int	check_quotes(const char *str)
{
	int single_quote_count = 0;
	int double_quote_count = 0;

	while (*str)
	{
		if (*str == '\'')
		{
			single_quote_count++;
		}
		else if (*str == '"')
		{
			double_quote_count++;
		}
		str++;
	}

	// Check if both counts are even
	if (single_quote_count % 2 == 0 && double_quote_count % 2 == 0)
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

void	initcmd(char *input, char **env, t_data *data)
{
	// check quote closed
	t_command *command;

	if (ft_strlen(input) == 0)
		return ;
	tokenizer(input, data);

	concatenvtoken(data);

	printcmds(data);
	remove_quotes(data->cmdchain);

	// if (!checksyntaxerror(data))
	// {
		command = parse_tokens(data->cmdchain);
		// print_command_list(command);
		execute_pipeline(command, data);
		free_command_list(command);
	// }
	free(input);
	free_cmdchain(data->cmdchain);
	(void)env;
	(void)command;
	(void)data;
}
