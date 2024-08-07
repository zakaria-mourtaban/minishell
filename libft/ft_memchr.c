/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:28:45 by zmourtab          #+#    #+#             */
/*   Updated: 2024/06/20 10:28:07 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	char				ch;
	const unsigned char	*str;

	ch = c;
	str = s;
	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)(str + i)) == (unsigned char)ch)
			return ((unsigned char *)str + i);
		i++;
	}
	return (NULL);
}
