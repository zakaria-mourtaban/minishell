/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksyntaxerror.c                                       :+:      :+: :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:26:21 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/16 17:12:09 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tokens	*getprev(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->previous;
	return (tmp);
}

t_tokens	*getnext(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	if (token == NULL)
		return (NULL);
	while (tmp && tmp->id == TOKEN_SPACE)
		tmp = tmp->next;
	return (tmp);
}

int	checkpipe(t_tokens *token)
{
	if (!getnext(token->next) || (getnext(token->next)->id != TOKEN_WORD
			&& getnext(token->next)->id != TOKEN_COMMAND)
		|| !getprev(token->previous)
		|| (getprev(token->previous)->id != TOKEN_WORD
			&& getprev(token->previous)->id != TOKEN_COMMAND))
		return (1);
	return (0);
}

int	checkfileoutappend(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_WORD)
		return (1);
	return (0);
}

int	checkheredoc(t_tokens *token)
{
	if (!getnext(token->next) || getnext(token->next)->id != TOKEN_COMMAND)
		return (1);
	return (0);
}

int	checkfileout(t_tokens *token)
{
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_WORD
			&& next_token->id != TOKEN_COMMAND))
		return (1);
	return (0);
}

int	checkfilein(t_tokens *token)
{
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_WORD
			&& next_token->id != TOKEN_COMMAND))
		return (1);
	return (0);
}

void	printerror(t_tokens *token)
{
	if (token == NULL)
		printf("bash: syntax error near unexpected token `newline'\n");
	else if (token->id == TOKEN_DIRECTORY)
		printf("bash: %s: Is a directory\n", getnext(token)->content);
	else
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			getnext(token)->content);
	}
}
int	is_command(t_tokens *token)
{
	return (access(token->content, F_OK) == 0 && access(token->content,
			X_OK) == 0);
}

int	isdirectory(t_tokens *token)
{
	DIR	*dir;

	printf("%s\n", token->content);
	dir = opendir(token->content);
	if (dir && (ft_strcmp(token->content, "/") || ft_strcmp(token->content,
				".")))
	{
		token->id = TOKEN_DIRECTORY;
		closedir(dir);
		return (1);
	}
	return (0);
}

int	contains_dot_or_slash(const char *str)
{
	while (*str)
	{
		if (*str == '.' || *str == '/')
		{
			return (1);
		}
		str++;
	}
	return (0);
}

void	check_path(const char *path, t_data *data)
{
	struct stat	statbuf;
	char		*str;

	str = get_path((char *)path, data->env_list);
	if (access(str, X_OK) != 0 && !contains_dot_or_slash(path)
		&& !is_builtin_command(str))
	{
		data->cmd.status = 127;
		printf("bash: %s: command not found\n", path);
	}
	else if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)
		&& !is_builtin_command(str))
	{
		data->cmd.status = 126;
		printf("bash: %s: is a directory\n", path);
	}
	else if (access(str, X_OK) != 0 && !S_ISDIR(statbuf.st_mode)
		&& !is_builtin_command(str))
	{
		data->cmd.status = 127;
		printf("bash: %s: No such file or directory\n", path);
	}
	free(str);
}

void	handle_error(t_tokens *token, t_data *data,
		int (*check_func)(t_tokens *), void (*error_func)(t_tokens *),
		int status)
{
	if (check_func(token))
	{
		data->cmd.status = status;
		error_func(token->next);
		token->error = 1;
		return ;
	}
}

int	checksyntaxerror(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_PIPE)
		{
			handle_error(tmp, data, checkpipe, printerror, 2);
			if (tmp->error)
				return (1);
		}
		else if (tmp->id == TOKEN_HEREDOC_EOF)
		{
			handle_error(tmp, data, checkheredoc, printerror, 2);
			if (tmp->error)
				return (1);
		}
		else if (tmp->id == TOKEN_IN_FILE)
		{
			handle_error(tmp, data, checkfilein, printerror, 2);
			if (tmp->error)
				return (1);
		}
		else if (tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE)
		{
			handle_error(tmp, data, checkfileout, printerror, 2);
			if (tmp->error)
				return (1);
		}
		else if (tmp->id == TOKEN_COMMAND)
		{
			check_path(tmp->content, data);
			tmp->error = 1;
		}
		tmp = tmp->next;
	}
	return (0);
}
