/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:00:34 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/23 00:15:54 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handlesignal(t_data *data)
{
	if (data->cmd.running == 1)
		kill(data->cmd.pid, SIGINT);
	data->cmd.running = 0;
}

void	interactivehandle_sigint(int sig)
{
	signalint = 1;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)sig;
}

// Function to handle SIGQUIT (Ctrl-\), which we will ignore
void	interactivehandle_sigquit(int sig)
{
	(void)sig;
}

void	noninteractivehandle_sigint(int sig)
{
	signalint = 1;
	printf("\n");
	(void)sig;
}

// Function to handle SIGQUIT (Ctrl-\), which we will ignore
void	noninteractivehandle_sigquit(int sig)
{
	(void)sig;
}

void	interactivemode(t_data *data, char **input)
{
	signal(SIGINT, interactivehandle_sigint);
	signal(SIGQUIT, interactivehandle_sigquit);
	while (1)
	{
		printf(BLUE_BRIGHT "%s" RESET BG_RED_BRIGHT "%d" RESET,
			get_env(data->env_list, "PWD"), singalint);
		*input = readline(GREEN "⫤ " RESET);
		if (input != NULL)
			break ;
		if (ft_strlen(*input) != 0)
		{
			break ;
		}
	}
	(void)data;
}

void	noninteractivemode(t_data *data, char **input)
{
	signal(SIGINT, noninteractivehandle_sigint);
	signal(SIGQUIT, noninteractivehandle_sigint);
	while (data->cmd.running == 1 && signalint != 1)
	{
		if (signalint == 1)
		{
			handlesignal(data);
			printf("\n");
			break ;
		}
	}
	(void)*input;
}
