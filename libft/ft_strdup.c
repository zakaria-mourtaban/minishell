/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:50:46 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/03 12:29:40 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*sr;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	sr = malloc(i + 1);
	if (sr == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		sr[i] = s[i];
		i++;
	}
	sr[i] = '\0';
	return (sr);
}

char	*ft_strnew(int len)
{
	char	*str;

	str = malloc(len * sizeof(char));
	return (str);
}
