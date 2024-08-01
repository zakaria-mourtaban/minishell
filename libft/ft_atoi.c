/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:14:52 by zmourtab          #+#    #+#             */
/*   Updated: 2024/06/10 13:36:25 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *c)
{
	int	i;
	int	nb;
	int	neg;

	i = 0;
	nb = 0;
	neg = 1;
	while (c[i] == ' ' || c[i] == '\t' || c[i] == '\v'
		|| c[i] == '\f' || c[i] == '\n' || c[i] == '\r')
		i++;
	if (c[i] == '-')
		neg = -1;
	if (c[i] == '-' || c[i] == '+')
		i++;
	while (c[i] >= 48 && c[i] <= 57 && c[i] != '\0')
	{
		nb *= 10;
		nb += ((int)c[i] - 48);
		i++;
	}
	return (nb * neg);
}