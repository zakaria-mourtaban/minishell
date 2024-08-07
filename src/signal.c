/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:00:34 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/07 15:20:50 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handlesignal(t_data *data)
{
	if (data->cmd.running == 1)
		kill(data->cmd.pid, SIGINT);
	signalint = 0;
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
