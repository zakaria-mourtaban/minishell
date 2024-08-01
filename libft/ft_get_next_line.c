/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:36:39 by zmourtab          #+#    #+#             */
/*   Updated: 2024/07/15 03:47:31 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"

static size_t	find_next_line_break(char *str, size_t i)
{
	char	*ptr;

	ptr = str + i;
	while (*ptr && *ptr != '\n')
		ptr++;
	if (*ptr == '\n')
		ptr++;
	return (ptr - str);
}

char	*get_substr(char	*buffer)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!*buffer)
		return (free(buffer), NULL);
	i = find_next_line_break(buffer, i);
	str = (char *)malloc((ft_strlen(buffer) - i) + 1);
	if (!str)
		return (free(str), NULL);
	while (*(buffer + i))
		str[j++] = buffer[i++];
	str[j] = '\0';
	if (!*str)
		return (free(str), free(buffer), NULL);
	free (buffer);
	return (str);
}	

char	*get_lines(char *buffer)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!buffer || *buffer == '\0')
		return (NULL);
	i = find_next_line_break(buffer, i);
	line = (char *)malloc(sizeof(char) * i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*free_and_null(char *buff1, char *buff2)
{
	free(buff1);
	free(buff2);
	buff2 = NULL;
	return (buff2);
}

char	*ft_get_next_line(int fd)
{
	char		*read_content;
	int			read_bytes;
	static char	*read_buffer;

	read_bytes = 1;
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	read_content = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!read_content)
		return (NULL);
	while (!(ft_strchr(read_buffer, '\n')) && read_bytes != 0)
	{
		read_bytes = read(fd, read_content, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			read_buffer = free_and_null(read_content, read_buffer);
			return (NULL);
		}
		*(read_content + read_bytes) = '\0';
		read_buffer = ft_strjoin(read_buffer, read_content);
	}
	free(read_content);
	read_content = get_lines(read_buffer);
	read_buffer = get_substr(read_buffer);
	return (read_content);
}

// int main()
// {
// 	int i = open("file.txt",O_RDONLY);
// 	char *buf;
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	free(buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	free(buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	free(buf);
// 	buf = get_next_line(i);
// 	printf("%s", buf);
// 	free(buf);
// 	return (0);
// }
