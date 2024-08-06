/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:00:34 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/06 14:59:14 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handlesignal(t_data *data)
{
	if (data->cmd.running == 1)
		kill(data->cmd.pid, SIGINT);
	g_signalint = 0;
}

void	handle_sigint(int sig)
{
	g_signalint = 1;
	printf("\n");
	rl_on_new_line();       // Move to a new line
	rl_replace_line("", 0); // Clear the current line
	rl_redisplay();         // Refresh the display to show the new promp
	(void)sig;
}

// Function to handle SIGQUIT (Ctrl-\), which we will ignore
void	handle_sigquit(int sig)
{
	(void)sig;
}
