/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/05 14:15:17 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	signalint;

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	art();
	using_history();
	while (1)
	{
		data.cmd.running = 0;
		if (signalint != 1)
			input = readline(">>>");
		else
			input = readline("");
		handlesignal(&data);
		if (input == NULL)
		{
			printf("\n");
			break ;
		}
		add_history(input);
		initcmd(input, env, &data);
		free(input);
	}
	(void)ac;
	(void)av;
	return (0);
}
