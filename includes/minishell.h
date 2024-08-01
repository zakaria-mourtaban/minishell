/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:57 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/01 11:21:41 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"

/* Standard Input/Output functions */
#include <stdio.h>    // printf, perror, strerror
#include <unistd.h>   // write, access, close, read, getcwd, chdir, isatty, ttyname, ttyslot, ioctl, dup, dup2, pipe, fork, execve
#include <stdlib.h>   // malloc, free, exit, getenv

/* System and Process Management */
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, waitpid, wait3, wait4
#include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill

/* File system operations */
#include <sys/stat.h>   // stat, lstat, fstat
#include <fcntl.h>      // open, unlink
#include <dirent.h>     // opendir, readdir, closedir

/* Terminal control */
#include <termios.h>    // tcsetattr, tcgetattr
#include <curses.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

/* Readline library */
#include <readline/readline.h> // readline
#include <readline/history.h>  // rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history

/* ascii art */
#include "art.h"

void	art();

#endif
