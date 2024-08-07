/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:41:46 by zmourtab          #+#    #+#             */
/*   Updated: 2024/06/13 13:35:49 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	writen(char c, int fd)
{
	write (fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		writen ('-', fd);
		writen ('2', fd);
		n = 147483648;
	}
	if (n < 0)
	{
		writen ('-', fd);
		n *= -1;
	}
	if (n > 9)
	{
		ft_putnbr_fd (n / 10, fd);
		writen ((n % 10) + '0', fd);
	}
	else
	{
		writen (n + '0', fd);
	}
}
