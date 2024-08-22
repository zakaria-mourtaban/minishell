/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env_$.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:30:14 by odib              #+#    #+#             */
/*   Updated: 2024/08/21 16:20:37 by zmourtab         ###   ########.fr       */
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

char	*get_env_copy(char *name, t_env *copy_env)
{
	t_env	*current;

	current = copy_env;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	replace_env(char **input, char *old, char *new)
{
	char	*pos;
	char	*new_input;

	if (!new)
		return ;
	pos = ft_strnstr(*input, old, ft_strlen(*input));
	if (!pos)
		return ;
	new_input = malloc(ft_strlen(*input) - ft_strlen(old) + ft_strlen(new) + 1);
	if (!new_input)
		return ;
	ft_strlcpy(new_input, *input, pos - *input + 1);
	ft_strlcat(new_input, new, (pos - *input) + ft_strlen(new) + 1);
	ft_strlcat(new_input, pos + ft_strlen(old), (pos - *input) + ft_strlen(new)
		+ 1);
	free(*input);
	*input = new_input;
}

pid_t	ft_getpid(void)
{
	int		fd;
	char	buffer[256];
	ssize_t	bytesRead;
	pid_t	pid;
	char	*line;

	pid = -1;
	// Open the /proc/self/status file
	fd = open("/proc/self/status", O_RDONLY);
	if (fd == -1)
	{
		return (-1); // Error opening file
	}
	// Read the file content
	bytesRead = read(fd, buffer, sizeof(buffer) - 1);
	if (bytesRead == -1)
	{
		close(fd);
		return (-1); // Error reading file
	}
	// Null-terminate the buffer to make it a proper string
	buffer[bytesRead] = '\0';
	// Look for the line starting with "Pid:"
	line = buffer;
	while (line)
	{
		if (strncmp(line, "Pid:", 4) == 0)
		{
			// Extract the PID from the line
			pid = (pid_t)strtol(line + 5, NULL, 10);
			break ;
		}
		// Move to the next line
		line = strchr(line, '\n');
		if (line)
		{
			line++; // Move past the newline character
		}
	}
	// Close the file
	close(fd);
	return (pid);
}

char	*handle_double_dollar(char *input)
{
	char	*pos;
	char	pid_str[12];
	pid_t	pid;

	pid = ft_getpid();
	sprintf(pid_str, "%d", pid);
	while ((pos = ft_strnstr(input, "$$", ft_strlen(input))) != NULL)
		replace_env(&input, "$$", pid_str);
	return (input);
}

// Function to remove all occurrences of a specific character from a string
char	*remove_char(char *str, char char_to_remove)
{
	int		len;
	char	*result;
	char	*ptr;
	int		i;

	if (!str)
		return (NULL);
	len = strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ptr = result;
	i = 0;
	while (i < len)
	{
		if (ft_isspace(str[i + 1]) && str[i] == char_to_remove)
		{
			*ptr++ = str[i++];
			*ptr++ = str[i++]; // Copy the escaped character
		}
		else if (str[i] != char_to_remove)
		{
			*ptr++ = str[i++];
		}
		else if (str[i] == char_to_remove && str[i + 1] == '\0')
		{
			*ptr++ = str[i++];
		}
		else
		{
			i++;
		}
	}
	*ptr = '\0';
	return (result);
}
int	ft_is_delimiter(char c)
{
	if (c == '$')
	{
		return (1);
	}
	if (c == '|' || c == '>' || c == '<')
	{
		return (1);
	}
	if (c == '\'' || c == '"')
	{
		return (1);
	}
	return (0);
}
// Function to parse and replace environment variables
char	*handle_dollar_sign(char *input, t_env *envp_head)
{
	int i = 0;
	int start, end;
	char *sub_env;
	input = handle_double_dollar(input);

	while (input[i])
	{
		if (input[i] == '$' && ft_isdigit(input[i + 1]))
		{
			sub_env = ft_substr(input, i, 2);
			replace_env(&input, sub_env, "");
			free(sub_env);
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			start = i;
			while ((input[i] && !ft_isspace(input[i])
					&& !ft_is_delimiter(input[i])) || input[i] == '_')
				i++;
			end = i;
			sub_env = ft_substr(input, start, end - start);
			char *env_value = get_env_copy(sub_env, envp_head);
			if (env_value)
				replace_env(&input, sub_env, env_value);
			else
				replace_env(&input, sub_env, "");
			free(sub_env);
		}
		else
			i++;
	}
	return (remove_char(input, '$'));
}