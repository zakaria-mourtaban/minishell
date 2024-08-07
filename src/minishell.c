/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/07 16:29:28 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	signalint;



int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	art();
	using_history();
	while (1)
	{
		interactivemode(&data, &input);
		if (input == NULL)
			break ;
		if (ft_strlen(input) != 0)
			add_history(input);
		noninteractivemode(&data, &input);
		initcmd(input, env, &data);
		free(input);
	}
	(void)ac;
	(void)av;
	return (0);
}
