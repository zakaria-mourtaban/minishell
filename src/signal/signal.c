/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:00:34 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/31 22:59:22 by zmourtab         ###   ########.fr       */
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
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	signalint = 130;
	(void)sig;
}

// Function to handle SIGQUIT (Ctrl-\), which we will ignore
void	interactivehandle_sigquit(int sig)
{
	(void)sig;
}

void	noninteractivehandle_sigint(int sig)
{
	printf("\n");
	signalint = 130;
	(void)sig;
}

// Function to handle SIGQUIT (Ctrl-\), which we will ignore
void	noninteractivehandle_sigquit(int sig)
{
	(void)sig;
}

char	*getinfo(t_data *data)
{
	char	*info;
	char	*sig;
	char	*pwd;

	pwd = get_env(data->env_list, "PWD");
	sig = ft_itoa(data->cmd.status);
	info = ft_strdup("");
	info = ft_strjoingnl(info, BLUE_BRIGHT);
	info = ft_strjoingnl(info, pwd);
	info = ft_strjoingnl(info, RESET);
	info = ft_strjoingnl(info, BG_RED_BRIGHT);
	info = ft_strjoingnl(info, sig);
	info = ft_strjoingnl(info, RESET);
	info = ft_strjoingnl(info, GREEN "⫤ " RESET);
	if (sig)
		free(sig);
	if (pwd != NULL)
		free(pwd);
	return (info);
}

void	interactivemode(t_data *data, char **input)
{
	char	*info;
	signal(SIGINT, interactivehandle_sigint);
	signal(SIGQUIT, interactivehandle_sigquit);
	
	while (1)
	{
		info = getinfo(data);
		
		
		*input = readline(info);
		free(info);
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
	while (data->cmd.running == 1 && signalint != 130)
	{
		if (signalint == 130)
		{
			handlesignal(data);
			printf("\n");
			break ;
		}
	}
	(void)*input;
}
