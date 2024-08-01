/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:42:57 by zmourtab          #+#    #+#             */
/*   Updated: 2024/06/11 18:18:43 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	j;

	if (!big && !len)
		return (NULL);
	if (!*little)
		return ((char *)big);
	while (*big && len--)
	{
		j = 0;
		while (*(big + j) == *(little + j)
			&& *(little + j) && j <= len)
		{
			if (!*(little + j + 1))
				return ((char *)big);
			j++;
		}
		big++;
	}
	return (NULL);
}
