/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:37:09 by zmourtab          #+#    #+#             */
/*   Updated: 2024/06/20 10:46:33 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	const unsigned char	*source;
	size_t				i;

	if (dest == NULL && src == NULL)
		return (NULL);
	dst = dest;
	source = src;
	i = 0;
	while (i < n)
	{
		dst[i] = source[i];
		i++;
	}
	return (dst);
}
