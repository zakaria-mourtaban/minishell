/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:57 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/07 16:27:44 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

/* Standard Input/Output functions */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* System and Process Management */
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

/* File system operations */
# include <dirent.h>
# include <fcntl.h>
# include <sys/stat.h>
/* Terminal control */
# include <curses.h>
# include <termios.h>

/* Readline library */
# include <readline/history.h>
# include <readline/readline.h>

/* ascii art */
# include "art.h"

typedef struct s_cmd
{
	char			**cmd;
	int				status;
	int				running;
	pid_t			pid;
}					t_cmd;

typedef struct s_data
{
	t_cmd			cmd;
}					t_data;

extern volatile int	signalint;
void				art(void);
char				*get_path(char *cmd, char **env);
void				initcmd(const char *input, char **env, t_data *data);
void				runcmd(const char *input, char **env, t_data *data);
void				noninteractivehandle_sigquit(int sig);
void				noninteractivehandle_sigint(int sig);
void				interactivehandle_sigquit(int sig);
void				interactivehandle_sigint(int sig);
void	interactivemode(t_data *data, char **input);
void	noninteractivemode(t_data *data, char **input);
void				handlesignal(t_data *data);
#endif
