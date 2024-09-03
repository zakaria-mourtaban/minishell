/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/01 17:06:09 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 17:06:09 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	free_tokens(t_tokens *head)
{
	t_tokens *current;
	t_tokens *next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

int	check_quotes(const char *str)
{
	int single_quote_count = 0;
	int double_quote_count = 0;

	while (*str)
	{
		if (*str == '\'')
			single_quote_count++;
		else if (*str == '"')
			double_quote_count++;
		str++;
	}
	if (single_quote_count % 2 == 0 && double_quote_count % 2 == 0)
		return (0);
	else
		return (1);
}

void	fixuptoken(t_data *data)
{
	t_tokens *tmp;
	// e_token token;
	int foundcmd;

	foundcmd = 0;
	tmp = data->cmdchain;
	while (tmp)
	{
		if ((tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE
				|| tmp->id == TOKEN_HEREDOC_EOF || tmp->id == TOKEN_IN_FILE))
		{
			tmp = tmp->next;
			if (tmp && tmp->id == TOKEN_SPACE)
				tmp = tmp->next;
			if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND))
				tmp->id = TOKEN_FILE;
		}
		if (tmp)
			tmp = tmp->next;
		if (tmp && tmp->id == TOKEN_SPACE)
			tmp = tmp->next;
	}
	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_COMMAND)
			foundcmd = 1;
		if (tmp->id == TOKEN_PIPE)
			foundcmd = 0;
		if (tmp->id == TOKEN_WORD && foundcmd == 0)
		{
			foundcmd = 1;
			tmp->id = TOKEN_COMMAND;
		}
		tmp = tmp->next;
	}
}

t_command	*getcommands(t_data *data)
{
	t_command *cmd_list;
	t_command *current_cmd;
	t_tokens *tmp;

	tmp = data->cmdchain;
	cmd_list = NULL;
	current_cmd = NULL;
	if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND))
	{
		if (!current_cmd)
		{
			current_cmd = create_command_node();
			if (!current_cmd)
				return (NULL);
		}
		add_argument(current_cmd, tmp->content);
		if (tmp->error == 1)
			current_cmd->error = 1;
	}
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND)
		{
			if (!current_cmd)
			{
				current_cmd = create_command_node();
				if (!current_cmd)
					return (NULL);
			}
			add_argument(current_cmd, tmp->content);
		}
		else if (tmp->id == TOKEN_PIPE)
		{
			if (current_cmd)
			{
				append_command_node(&cmd_list, current_cmd);
				current_cmd = NULL;
			}
		}
		if (tmp && tmp->error == 1 && current_cmd)
			current_cmd->error = 1;
		if (tmp != NULL)
			tmp = tmp->next;
	}
	if (current_cmd)
		append_command_node(&cmd_list, current_cmd);
	return (cmd_list);
}

void	handleredirects(t_data *data, t_command *command)
{
	t_tokens *tmp;
	t_command *current_cmd;

	current_cmd = command;
	tmp = data->cmdchain;
	while (current_cmd)
	{
		while (tmp)
		{
			if (tmp->id == TOKEN_IN_FILE && tmp->error == 0)
			{
				tmp = tmp->next;
				if (tmp && tmp->id == TOKEN_SPACE)
					tmp = tmp->next;
				if (tmp && (tmp->id == TOKEN_FILE))
				{
					current_cmd->infile = open(tmp->content, O_RDONLY);
					if (current_cmd->infile == -1)
					{
						printf("bash: %s: Permission denied\n", tmp->content);
						current_cmd->error = 1;
						data->cmd.status = 1;
						break ;
					}
				}
				if (tmp)
					tmp = tmp->next;
			}
			else if (tmp->id == TOKEN_OUT_FILE && tmp->error == 0)
			{
				tmp = tmp->next;
				if (tmp->id == TOKEN_SPACE)
					tmp = tmp->next; // Move to the next token,
				if (tmp && (tmp->id == TOKEN_FILE))
				{
					current_cmd->outfile = open(tmp->content,
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (current_cmd->outfile == -1)
					{
						printf("bash: %s: Permission denied\n", tmp->content);
						current_cmd->error = 1;
						data->cmd.status = 1;
						break ;
					}
				}
				tmp = tmp->next;
			}
			else if (tmp->id == TOKEN_OUT_A_FILE && tmp->error == 0)
			{
				tmp = tmp->next;
				if (tmp->id == TOKEN_SPACE)
					tmp = tmp->next; // Move to the next token,
				if (tmp && (tmp->id == TOKEN_FILE))
				{
					current_cmd->outfile = open(tmp->content,
							O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (current_cmd->outfile == -1)
					{
						printf("bash: %s: Permission denied\n", tmp->content);
						data->cmd.status = 1;
						current_cmd->error = 1;
						break ;
					}
				}
				tmp = tmp->next;
			}
			if (tmp && tmp->id == TOKEN_PIPE)
				break ;
			if (tmp)
				tmp = tmp->next;
		}
		if (tmp != NULL)
			tmp = tmp->next;
		if (current_cmd != NULL)
			current_cmd = current_cmd->next;
	}
}

void	handleheredoc(t_data *data)
{
	t_tokens *tmp;
	char *input;
	char *tmpstr;
	char *buffer;
	int i;

	input = NULL;
	buffer = NULL;
	signal(SIGINT, interactivehandle_sigint);
	signal(SIGQUIT, interactivehandle_sigquit);
	tmp = data->cmdchain;
	i = 0;
	while (tmp)
	{
		if (tmp->id == TOKEN_HEREDOC_EOF && tmp->error == 0)
		{
			tmp->id = TOKEN_IN_FILE;
			free(tmp->content);
			tmp->content = ft_strdup("<");
			tmp = tmp->next;
			if (tmp && tmp->id == TOKEN_SPACE)
				tmp = tmp->next;
			if (tmp && (tmp->id == TOKEN_FILE) && tmp->error == 0)
			{
				while (1)
				{
					input = readline("> ");
					if (input && ft_strcmp(input, tmp->content) == 0
						&& ft_strlen(input) == ft_strlen(tmp->content))
						break ;
					if (input == NULL)
						break ;
					if (buffer == NULL)
						buffer = ft_strdup(input);
					else
						buffer = ft_strjoingnl(buffer, input);
					buffer = ft_strjoingnl(buffer, "\n");
					free(input);
				}
				if (input != NULL)
					printf("%s\n", input);
			}
			else
				break ;
			if (buffer == NULL)
				buffer = ft_strdup("");
			tmpstr = ft_itoa(i);
			data->tmpfd = open(tmpstr, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (buffer)
				ft_putstr_fd(buffer, data->tmpfd);
			close(data->tmpfd);
			if (buffer)
				free(buffer);
			tmp->content = ft_strdup(tmpstr);
			free(tmpstr);
			tmp = tmp->next;
		}
		i++;
		if (tmp && tmp->id == TOKEN_PIPE)
			break ;
		if (tmp)
			tmp = tmp->next;
	}
	signal(SIGINT, noninteractivehandle_sigint);
	signal(SIGQUIT, noninteractivehandle_sigquit);
}

int	checkheredocerror(t_data *data)
{
	t_tokens *tmp;
	int haserrored;
	haserrored = 0;
	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_HEREDOC_EOF && checkheredoc(tmp))
		{
			printf("here\n");
			data->cmd.status = 2;
			printerror(tmp);
			tmp->error = 1;
			tmp = getnextcommand(tmp);
			haserrored = 1;
			continue ;
		}
		if (tmp)
			tmp = tmp->next;
	}
	return (haserrored);
}

void	initcmd(char *input, char **env, t_data *data)
{
	t_command *command;

	signalint = 0;
	if (ft_strlen(input) == 0)
		return ;
	tokenizer(handle_dollar_sign(input, data), data);
	fixuptoken(data);
	remove_quotes(data->cmdchain);
	if (!checkheredocerror(data))
	{
		handleheredoc(data);
		checksyntaxerror(data);
		printcmds(data);
		command = getcommands(data);
		handleredirects(data, command);
		print_command_list(command);
		execute_pipeline(command, data);
		free_command_list(command);
	}
	free_cmdchain(data->cmdchain);
	if (data->cmd.status > 255)
		data->cmd.status = data->cmd.status % 255;
	// free(input);
	(void)env;
	(void)command;
	(void)data;
}
// free(input);
// checksyntaxerror(data);
// command = parse_tokens(data->cmdchain);
// execute_pipeline(command, data);
