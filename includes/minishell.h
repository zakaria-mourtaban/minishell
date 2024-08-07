/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:57 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/07 15:13:43 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"

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
void				handlesignal(t_data *data);



//ahmad 

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_ERROR,
    TOKEN_EOF
} t_tokens_type;

typedef struct s_token
{
	char            *type;
	char			*value;
	struct	s_token	*next;
}	t_list_tokens;

typedef struct s_command
{
    t_list_tokens tokens_list;
    
}   t_command;

int					ft_iswhitespace(char c);
void				*ft_skip_spaces(char *str);
char				*strndup(const char *s, size_t n);
t_list_tokens		*add_token(t_list_tokens **tokens, t_tokens_type type, char *value);
char				*collect_word(char **input);
int					*tokenize(char *input);
void				free_tokens(t_list_tokens *tokens);

#endif




