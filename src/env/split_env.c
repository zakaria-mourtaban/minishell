/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:43:08 by odib              #+#    #+#             */
/*   Updated: 2024/08/31 14:37:22 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_strnlen(const char *str, size_t n)
{
	size_t	length;

	length = 0;
	while (length < n && str[length] != '\0')
		length++;
	return (length);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (src[i] != '\0')
			dest[i] = src[i];
		else
			dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*new_str;

	len = ft_strnlen(s, n);
	new_str = malloc(len + 1);
	if (!new_str)
		return (NULL);
	ft_strncpy(new_str, s, len);
	new_str[len] = '\0';
	return (new_str);
}

int	ft_strlen1(const char *s)
{
	int	len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

void	split_envp(char *envp_str, char **key, char **value)
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
		*value = ft_strdup("");
	}
}

// static char	**allocate_result_array(void)
// {
// 	char	**res;

// 	res = (char **)malloc(sizeof(char *) * 3);
// 	if (!res)
// 		return (NULL);
// 	res[2] = NULL;
// 	return (res);
// }

// static char	*extract_key(char *str, char del)
// {
// 	int		i;
// 	char	*key;

// 	i = 0;
// 	while (str[i] && str[i] != del)
// 		i++;
// 	key = (char *)malloc(sizeof(char) * (i + 1));
// 	if (!key)
// 		return (NULL);
// 	i = 0;
// 	while (str[i] && str[i] != del)
// 	{
// 		key[i] = str[i];
// 		i++;
// 	}
// 	key[i] = '\0';
// 	return (key);
// }

// static char	*extract_value(char *str, char del)
// {
// 	int		i;
// 	int		j;
// 	char	*value;

// 	i = 0;
// 	while (str[i] && str[i] != del)
// 		i++;
// 	if (str[i] == del)
// 		i++;
// 	j = 0;
// 	while (str[i + j] != '\0')
// 		j++;
// 	value = (char *)malloc(sizeof(char) * (j + 1));
// 	if (!value)
// 		return (NULL);
// 	j = 0;
// 	while (str[i])
// 	{
// 		value[j] = str[i];
// 		i++;
// 		j++;
// 	}
// 	value[j] = '\0';
// 	return (value);
// }

// char	**ft_split_env(char *str, char del)
// {
// 	char **res = allocate_result_array();
// 	if (!res)
// 		return (NULL);

// 	res[0] = extract_key(str, del);
// 	if (!res[0])
// 		return (NULL);

// 	res[1] = extract_value(str, del);
// 	if (!res)
// 		return (NULL);
// 	return (res);
// }