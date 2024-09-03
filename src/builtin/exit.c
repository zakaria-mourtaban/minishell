/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:07:54 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 20:37:18 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_status_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	calculate_exit_status(int status)
{
	int res;
	
	res = status % 256;
	while (res < 0)
	{
		res += 256;
	}
	return (res);
}

int	convert_and_validate_exit_status(char *arg)
{
	int exit_status;
	
	if (!check_status_str(arg))
	{
		printf("exit : %s numeric argument needed\n", arg);
		return (1);
	}
	exit_status = ft_atoi(arg);
	return (calculate_exit_status(exit_status));
}

void	handle_too_many_args_error()
{
	printf("exit: too many arguments\n");
	exit(1);
}

int	exit_command(t_arg *args)
{
	int exit_status;
	t_arg *arg;

	arg = args->next;
	if (arg && arg->next)
		handle_too_many_args_error();
	if (arg)
		exit_status = convert_and_validate_exit_status(arg->arg);
	else
		exit_status = 0;
	return (exit(exit_status), exit_status);
}
