/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env_$.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:30:14 by odib              #+#    #+#             */
/*   Updated: 2024/08/31 23:06:39 by zmourtab         ###   ########.fr       */
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
	//free(*result);
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
char	*handle_dollar_sign(char *input, t_data *data)
{
	int		i;
	char	*result;
	char	tmp[2];

	i = 0;
	result = ft_strdup("");
	while (i < ft_strlen1(input) && input[i])
		if (input[i] == '$')
		{
			if (input[i + 1] == '$')
				handle_two_dollar(&result, &i);
			else if (input[i + 1] == '?')
			{
				result = ft_strjoingnl(result, ft_itoa(data->cmd.status));
				i += 2;
			}
			else if (ft_isdigit(input[i + 1]))
				i += 2;
			else
				handle_normal_variable(input, &i, &result, data->env_list);
		}
		else
		{
			ft_strlcpy(tmp, input + i, 2);
			result = ft_strjoingnl(result, tmp);
			i++;
		}
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

// char	*get_env_copy(char *name, t_env *copy_env)
// {
// 	t_env	*current;

// 	current = copy_env;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->key, name) == 0)
// 			return (current->value);
// 		current = current->next;
// 	}
// 	return (NULL);
// }

// void	replace_env(char **input, char *old, char *new)
// {
// 	char	*pos;
// 	char	*new_input;

// 	if (!new)
// 		return ;
// 	pos = ft_strnstr(*input, old, ft_strlen(*input));
// 	if (!pos)
// 		return ;
// 	new_input = malloc(ft_strlen(*input) - ft_strlen(old) + ft_strlen(new) + 1);
// 	if (!new_input)
// 		return ;
// 	ft_strlcpy(new_input, *input, pos - *input + 1);
// 	ft_strlcat(new_input, new, (pos - *input) + ft_strlen(new) + 1);
// 	ft_strlcat(new_input, pos + ft_strlen(old), (pos - *input) + ft_strlen(new)
// 		+ 1);
// 	free(*input);
// 	*input = new_input;
// }

// pid_t	ft_getpid(void)
// {
// 	int		fd;
// 	char	buffer[256];
// 	ssize_t	bytesRead;
// 	pid_t	pid;
// 	char	*line;

// 	pid = -1;
// 	// Open the /proc/self/status file
// 	fd = open("/proc/self/status", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		return (-1); // Error opening file
// 	}
// 	// Read the file content
// 	bytesRead = read(fd, buffer, sizeof(buffer) - 1);
// 	if (bytesRead == -1)
// 	{
// 		close(fd);
// 		return (-1); // Error reading file
// 	}
// 	// Null-terminate the buffer to make it a proper string
// 	buffer[bytesRead] = '\0';
// 	// Look for the line starting with "Pid:"
// 	line = buffer;
// 	while (line)
// 	{
// 		if (strncmp(line, "Pid:", 4) == 0)
// 		{
// 			// Extract the PID from the line
// 			pid = (pid_t)strtol(line + 5, NULL, 10);
// 			break ;
// 		}
// 		// Move to the next line
// 		line = strchr(line, '\n');
// 		if (line)
// 		{
// 			line++; // Move past the newline character
// 		}
// 	}
// 	// Close the file
// 	close(fd);
// 	return (pid);
// }

// char	*handle_double_dollar(char *input)
// {
// 	char	*pos;
// 	char	pid_str[12];
// 	pid_t	pid;

// 	pid = ft_getpid();
// 	sprintf(pid_str, "%d", pid);
// 	while ((pos = ft_strnstr(input, "$$", ft_strlen(input))) != NULL)
// 		replace_env(&input, "$$", pid_str);
// 	return (input);
// }

// // Function to remove all occurrences of a specific character from a string
// char	*remove_char(char *str, char char_to_remove)
// {
// 	int		len;
// 	char	*result;
// 	char	*ptr;
// 	int		i;

// 	if (!str)
// 		return (NULL);
// 	len = strlen(str);
// 	result = malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	ptr = result;
// 	i = 0;
// 	while (i < len)
// 	{
// 		if (ft_isspace(str[i + 1]) && str[i] == char_to_remove)
// 		{
// 			*ptr++ = str[i++];
// 			*ptr++ = str[i++]; // Copy the escaped character
// 		}
// 		else if (str[i] != char_to_remove)
// 		{
// 			*ptr++ = str[i++];
// 		}
// 		else if (str[i] == char_to_remove && str[i + 1] == '\0')
// 		{
// 			*ptr++ = str[i++];
// 		}
// 		else
// 		{
// 			i++;
// 		}
// 	}
// 	*ptr = '\0';
// 	return (result);
// }
// int	ft_is_delimiter(char c)
// {
// 	if (c == '$')
// 	{
// 		return (1);
// 	}
// 	if (c == '|' || c == '>' || c == '<')
// 	{
// 		return (1);
// 	}
// 	if (c == '\'' || c == '"')
// 	{
// 		return (1);
// 	}
// 	return (0);
// }
// // Function to parse and replace environment variables
// char	*handle_dollar_sign(char *input, t_env *envp_head)
// {
// 	int i = 0;
// 	int start, end;
// 	char *sub_env;
// 	input = handle_double_dollar(input);

// 	while (input[i])
// 	{
// 		if (input[i] == '$' && ft_isdigit(input[i + 1]))
// 		{
// 			sub_env = ft_substr(input, i, 2);
// 			replace_env(&input, sub_env, "");
// 			free(sub_env);
// 			i += 2;
// 		}
// 		else if (input[i] == '$')
// 		{
// 			i++;
// 			start = i;
// 			while ((input[i] && !ft_isspace(input[i])
// 					&& !ft_is_delimiter(input[i])) || input[i] == '_')
// 				i++;
// 			end = i;
// 			sub_env = ft_substr(input, start, end - start);
// 			char *env_value = get_env_copy(sub_env, envp_head);
// 			if (env_value)
// 				replace_env(&input, sub_env, env_value);
// 			else
// 				replace_env(&input, sub_env, "");
// 			free(sub_env);
// 		}
// 		else
// 			i++;
// 	}
// 	return (remove_char(input, '$'));
// }