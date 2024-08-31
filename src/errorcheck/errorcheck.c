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
		return (1); // Error: No valid file or command after >
	return (0);     // No error
}

int	checkfilein(t_tokens *token)
{
	t_tokens	*next_token;

	next_token = getnext(token->next);
	if (!next_token || (next_token->id != TOKEN_WORD
			&& next_token->id != TOKEN_COMMAND))
		return (1); // Error: No valid file or command after <
	return (0);     // No error
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
	// Check if the file exists and is executable
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
	{ // Iterate through the string
		if (*str == '.' || *str == '/')
		{
			return (1); // Return 1 if '.' or '/' is found
		}
		str++;
	}
	return (0); // Return 0 if neither '.' nor '/' is found
}

void	check_path(const char *path, t_data *data)
{
	struct stat	statbuf;
	char		*str;

	str = get_path((char *)path, data->env_list);
	if (access(str, X_OK) != 0 && !contains_dot_or_slash(path))
	{
		data->cmd.status = 127;
		printf("bash: %s: command not found\n", path);
	}
	else if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		data->cmd.status = 126;
		printf("bash: %s: is a directory\n", path);
	}
	else if (access(str, X_OK) != 0 && !S_ISDIR(statbuf.st_mode))
	{
		data->cmd.status = 127;
		printf("bash: %s: No such file or directory\n", path);
	}
	free(str);
}

int	checksyntaxerror(t_data *data)
{
	t_tokens	*tmp;

	tmp = data->cmdchain;
	while (tmp)
	{
		if (tmp->id == TOKEN_PIPE && checkpipe(tmp))
		{
			data->cmd.status = 2;
			printerror(tmp);
			tmp->error = 1;
			tmp = tmp->next;
			return (1);
			// return (1);
		}
		if (tmp->id == TOKEN_HEREDOC_EOF && checkheredoc(tmp))
		{
			data->cmd.status = 2;
			printerror(tmp->next);
			tmp->error = 1;
			tmp = tmp->next;
			return (1);
			// return (1);
		}
		if (tmp->id == TOKEN_IN_FILE && checkfilein(tmp))
		{
			data->cmd.status = 2;
			printerror(tmp->next);
			tmp->error = 1;
			tmp = tmp->next;
			return (1);
			// return (1);
		}
		if (tmp->id == TOKEN_IN_FILE && checkfilein(tmp))
		{
			data->cmd.status = 2;
			printerror(tmp->next);
			tmp->error = 1;
			tmp = tmp->next;
			return (1);
			// return (1);
		}
		if ((tmp->id == TOKEN_OUT_FILE || tmp->id == TOKEN_OUT_A_FILE)
			&& checkfileout(tmp))
		{
			data->cmd.status = 2;
			printerror(tmp->next);
			tmp->error = 1;
			tmp = tmp->next;
			return (1);
			// return (1);
		}
		if (tmp->id == TOKEN_COMMAND)
		{
			check_path(tmp->content, data);
			tmp->error = 1;
			tmp = tmp->next;
			continue ;
		}
		// printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	return (0);
}
// tmp = data->cmdchain;
// while (tmp)
// {
// 	switch (tmp->id)
// 	{
// 	case TOKEN_COMMAND:
// 		if (isdirectory(tmp))
// 		{
// 			printerror(tmp);
// 			return (1);
// 		}
// 		break ;
// 	case (TOKEN_OUT_FILE):
// 		if (checkfileout(tmp))
// 		{
// 			printerror(tmp);
// 			return (1);
// 		}
// 		break ;
// 	case (TOKEN_OUT_A_FILE):
// 		if (checkfileout(tmp))
// 		{
// 			printerror(tmp);
// 			return (1);
// 		}
// 		break ;
// 	case TOKEN_IN_FILE:
// 		if (checkfilein(tmp))
// 		{
// 			printerror(tmp);
// 			return (1);
// 		}
// 		break ;
// 	case TOKEN_HEREDOC_EOF:
// 		if (checkheredoc(tmp))
// 		{
// 			printerror(tmp);
// 			return (1);
// 		}
// 		break ;
// 	default:
// 		break ;
// 	}
// 	tmp = tmp->next;
// }
