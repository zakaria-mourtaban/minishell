/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env_$.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 23:30:14 by odib              #+#    #+#             */
/*   Updated: 2024/08/23 00:15:54 by zmourtab         ###   ########.fr       */
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
	return (ft_strnstr(buffer, "Pid:",4));
}

ssize_t read_status_file(char *buffer, size_t size)
{
    int fd = open("/proc/self/status", O_RDONLY);
    ssize_t bytesRead = -1;
    if (fd != -1)
    {
        bytesRead = read(fd, buffer, size - 1);
        close(fd);
    }
    return (bytesRead);
}

pid_t ft_getuid()
{
    char buffer[256];
    ssize_t bytesRead;
    char *uid_line;
    pid_t uid = -1;

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
pid_t	ft_getpid()
{
	int		fd;
	char	*line;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd == -1) {
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

char	*get_env(t_env *head, const char *key)
{
	if (!ft_strcmp(key,"UID") && ft_strlen(key) == 3)
		return (ft_itoa((int)ft_getuid()));
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0 && ft_strlen(head->key) == ft_strlen(key)) 
			return head->value;
		head = head->next;
	}
	return NULL;
}

void handle_normal_variable(char *input, int *i, char **result, t_env *env)
{
	int start;
	int end;
	char *sub_env;
	char *env_value;

	(*i)++;
	start = *i;
	if (*result == NULL)
		*result = ft_strdup("");
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	end = *i;
	if (start != end)
	{
		sub_env = ft_substr(input, start, end - start);
        env_value = get_env(env, sub_env);
        if (env_value)
            *result = ft_strjoingnl(*result, env_value);  // Append the env value to result
        free(sub_env);
	}
	else
        *result = ft_strjoingnl(*result, "$");
}
char	*handle_two_dollar(char *input, int *i)
{
	char	*result;
	char	*num_str;

	result = ft_substr(input, 0, *i);
	num_str = ft_itoa(getpid());
	result = ft_strjoingnl(result, num_str);
	result = ft_strjoingnl(result, (input) + (*i) + 2);
	*i += ft_strlen(num_str);
	free(num_str);
	return (result);
}
char	*handle_dollar_sign(char *input, t_data *data)
{
	int		i;
	char	*result;
	char	tmp[2];

	i = 0;
	result = NULL;
	while (i < (int)ft_strlen(input) && input[i])
	{
		
		if (input[i] == '$')
		{
			if (input[i + 1] == '\'' || input[i + 1] == '\"')
				i++;
			else if (input[i + 1] == '?')
			{
				result = ft_itoa(singalint);
				i+=2;
			}
			else if (input[i + 1] == '$')
			{
				result = ft_itoa((int)ft_getpid());
				i+=2;
			}
			else if (input[i + 1] == '$')
				result = handle_two_dollar(input, &i);
			else if (ft_isdigit(input[i + 1]))
				i += 2;
			else
				handle_normal_variable(input, &i, &result, data->env_list);
		}
		else
		{
			ft_strlcpy(tmp, input + i, 2);
			result = ft_strjoin(result, tmp);
			i++;
		}
	}
	if (result == NULL)
		return (input);
	free(input);
	return (result);
}

// t_tokens	*dollar_expansion(t_tokens *tokens_list, t_env *env)
// {
// 	char		*str;
// 	t_tokens	*current_token;
	
// 	current_token = tokens_list;
// 	while (current_token)
// 	{
// 		if (current_token->content && ft_strchr(current_token->content, '$'))
// 		{
// 			str = ft_strdup(current_token->content);
// 			free(current_token->content);
// 			current_token->content = handle_dollar_sign(str, data);
// 		}current_token = current_token->next;
// 	}
// 	return (tokens_list);
// }