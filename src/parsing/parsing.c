/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:05:11 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/04 14:15:34 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tokens	*handle_out_file_token(t_tokens *tmp, t_command *current_cmd,
			int append)
{
	tmp = tmp->next;
	if (tmp->id == TOKEN_SPACE)
		tmp = tmp->next;
	if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND)
		&& current_cmd && !current_cmd->error)
	{
		if (append)
		{
			current_cmd->outfile = open(tmp->content,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
		{
			current_cmd->outfile = open(tmp->content,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		current_cmd->append = append;
	}
	return (nexttoken(tmp));
}

t_command	*initialize_command_list(t_tokens *tokens)
{
	t_command	*current_cmd;

	current_cmd = NULL;
	if (tokens && (tokens->id == TOKEN_WORD || tokens->id == TOKEN_COMMAND))
		handle_word_command_token(tokens, &current_cmd);
	return (current_cmd);
}

void	process_token(t_tokens **tmp, t_command **cmd_list,
		t_command **current_cmd)
{
	if ((*tmp)->error == 1 && *current_cmd)
		(*current_cmd)->error = 1;
	if ((*tmp)->id == TOKEN_WORD || (*tmp)->id == TOKEN_COMMAND)
		handle_word_command_token(*tmp, current_cmd);
	else if ((*tmp)->id == TOKEN_PIPE)
		handle_pipe_token(cmd_list, current_cmd);
	else if ((*tmp)->id == TOKEN_IN_FILE)
		*tmp = handle_in_file_token(*tmp, *current_cmd);
	else if ((*tmp)->id == TOKEN_OUT_FILE)
		*tmp = handle_out_file_token(*tmp, *current_cmd, 0);
	else if ((*tmp)->id == TOKEN_OUT_A_FILE)
		*tmp = handle_out_file_token(*tmp, *current_cmd, 1);
	if (*tmp != NULL)
		*tmp = (*tmp)->next;
}

void	process_tokens_loop(t_tokens *tokens, t_command **cmd_list,
		t_command **current_cmd)
{
	t_tokens	*tmp;

	tmp = tokens->next;
	while (tmp)
	{
		process_token(&tmp, cmd_list, current_cmd);
	}
}

t_command	*parse_tokens(t_tokens *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;

	cmd_list = NULL;
	current_cmd = initialize_command_list(tokens);
	process_tokens_loop(tokens, &cmd_list, &current_cmd);
	if (current_cmd)
		append_command_node(&cmd_list, current_cmd);
	free_command_list(current_cmd);
	return (cmd_list);
}

// t_command	*parse_tokens(t_tokens *tokens)
// {
// 	t_command	*cmd_list = NULL;
// 	t_command	*current_cmd = NULL;
// 	t_tokens	*tmp = tokens;
//
// 	if (tmp && (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND))
// 		handle_word_command_token(tmp, &current_cmd);
// 	tmp = tmp->next;
// 	while (tmp)
// 	{
// 		if (tmp->error == 1 && current_cmd)
// 			current_cmd->error = 1;
// 		if (tmp->id == TOKEN_WORD || tmp->id == TOKEN_COMMAND)
// 			handle_word_command_token(tmp, &current_cmd);
// 		else if (tmp->id == TOKEN_PIPE)
// 			handle_pipe_token(&cmd_list, &current_cmd);
// 		else if (tmp->id == TOKEN_IN_FILE)
// 			tmp = handle_in_file_token(tmp, current_cmd);
// 		else if (tmp->id == TOKEN_OUT_FILE)
// 			tmp = handle_out_file_token(tmp, current_cmd, 0);
// 		else if (tmp->id == TOKEN_OUT_A_FILE)
// 			tmp = handle_out_file_token(tmp, current_cmd, 1);
// 		if (tmp != NULL)
// 			tmp = tmp->next;
// 	}
// 	if (current_cmd)
// 		append_command_node(&cmd_list, current_cmd);
// 	return (cmd_list);
// }
