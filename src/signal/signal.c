/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 00:00:34 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/03 11:52:38 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
