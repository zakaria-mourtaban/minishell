/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env_$.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:30:14 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 02:53:17 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*read_pid_line(int fd)
{
	static char	buffer[256];
	ssize_t		bytesRead;

	bytesRead = read(fd, buffer, sizeof(buffer) - 1);
	if (bytesRead <= 0)
		return (NULL);
	buffer[bytesRead] = '\0';
	return (strstr(buffer, "Pid:"));
}

void	ft_split_env(char *envp_str, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(envp_str, '=');
	if (equal_sign)
	{
		*key = ft_strndup(envp_str, equal_sign - envp_str);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(envp_str);
		*value = NULL;
	}
}

pid_t	ft_getpid(void)
{
	int		fd;
	char	*line;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	line = read_pid_line(fd);
	close(fd);
	if (line != NULL)
		return ((pid_t)ft_atoi(line + 4));
	else
		return (-1);
}

ssize_t	read_status_file(char *buffer, size_t size)
{
	int		fd;
	ssize_t	bytesRead;

	fd = open("/proc/self/status", O_RDONLY);
	bytesRead = -1;
	if (fd != -1)
	{
		bytesRead = read(fd, buffer, size - 1);
		close(fd);
	}
	return (bytesRead);
}

pid_t	ft_getuid(void)
{
	char	buffer[256];
	ssize_t	bytesRead;
	char	*uid_line;
	pid_t	uid;

	uid = -1;
	bytesRead = read_status_file(buffer, sizeof(buffer));
	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		uid_line = ft_strstr(buffer, "Uid:");
		if (uid_line)
		{
			uid_line = ft_strchr(uid_line, '\t');
			if (uid_line)
				uid = ft_atoi(uid_line + 1);
		}
	}
	return (uid);
}
void	handle_normal_variable(char *input, int *i, char **result, t_env *env)
{
	int		start;
	int		end;
	char	*sub_env;
	char	*env_value;

	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	end = *i;
	if (start != end)
	{
		sub_env = ft_substr(input, start, end - start);
		env_value = get_env(env, sub_env);
		if (env_value && *result != NULL)
			*result = ft_strjoingnl(*result, env_value);
		else
			*result = ft_strdup(env_value);
		free(env_value);
		free(sub_env);
	}
	else
		*result = ft_strjoingnl(*result, "$");
}

void	handle_two_dollar(char **result, int *i)
{
	char	*num_str;

	num_str = ft_itoa(getpid());
	*result = ft_strjoingnl(*result, num_str);
	*i += 2;
	free(num_str);
}

void	handle_question_mark(char **result, int *i, t_data *data)
{
	*result = ft_strjoingnl(*result, ft_itoa(data->cmd.status));
	(*i) += 2;
}

void	copy_and_append_char(char **result,char *input, int *i)
{
	char	tmp[2];

	tmp[0] = input[*i];
	tmp[1] = '\0';
	*result = ft_strjoingnl(*result, tmp);
	(*i)++;
}

char	*handle_dollar_sign(char *input, t_data *data)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (i < ft_strlen1(input) && input[i])
		if (input[i] == '$')
		{
			if (input[i + 1] == '$')
				handle_two_dollar(&result, &i);
			else if (input[i + 1] == '?')
				handle_question_mark(&result, &i, data);
			else if (ft_isdigit(input[i + 1]))
				i += 2;
			else
				handle_normal_variable(input, &i, &result, data->env_list);
		}
		else
			copy_and_append_char(&result, input, &i);
	free(input);
	return (result);
}

t_tokens	*dollar_expansion(t_tokens *tokens_list, t_data *data)
{
	t_tokens	*current_token;

	current_token = tokens_list;
	while (current_token)
	{
		if (current_token->content && ft_strchr(current_token->content, '$'))
			current_token->content = handle_dollar_sign(current_token->content,
					data);
		current_token = current_token->next;
	}
	return (tokens_list);
}
