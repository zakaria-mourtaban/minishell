/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:15:38 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/16 17:16:57 by zmourtab         ###   ########.fr       */
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

//#$%^&**@!#&******######!!@@^^*(_++)

typedef enum s_token
{

	TOKEN_WORD = 1,
	TOKEN_IN_FILE = 2,
	TOKEN_OUT_FILE = 3,
	TOKEN_HEREDOC_EOF = 4,
	TOKEN_OUT_A_FILE = 5,
	TOKEN_COMMAND = 6,
	TOKEN_OPERATOR = 7,
	TOKEN_PIPE = 8,
	TOKEN_SPACE = 9
}						e_token;

typedef struct s_tokens
{
	e_token				id;
	char				*content;
	struct s_tokens		*next;
	struct s_tokens		*previous;
}						t_tokens;

void					remove_quotes(t_tokens *tokens);
void					specify_token_cmd(t_tokens *token);

//#$%^&**@!#&******######!!@@^^*(_++)

/* ascii art */
# include "art.h"

typedef struct s_arg
{
	char				*arg;
	struct s_arg		*next;
}						t_arg;

typedef struct s_command
{
	t_arg				*args;
	int					infile;
	int					outfile;
	struct s_command	*next;
}						t_command;

typedef struct s_value
{
	char				*value;
	struct s_value		*next;
}						t_value;

typedef struct s_env
{
	char				*key;
	t_value				*value_head;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**cmd;
	int					status;
	int					running;
	pid_t				pid;
}						t_cmd;

typedef struct s_data
{
	t_env				*env_list;
	t_tokens			*cmdchain;
	t_cmd				cmd;
	int					errorid;
}						t_data;

extern volatile int		signalint;
void					art(void);
void					free_data(t_data *data);
void					tokenizer(char *input, t_data *data);
void					printcmds(t_data *data);
char					*get_path(char *cmd, char **env);
void					initcmd(char *input, char **env, t_data *data);
void					runcmd(const char *input, char **env, t_data *data);
void					noninteractivehandle_sigquit(int sig);
void					noninteractivehandle_sigint(int sig);
void					interactivehandle_sigquit(int sig);
void					interactivehandle_sigint(int sig);
void					interactivemode(t_data *data, char **input);
void					noninteractivemode(t_data *data, char **input);
char					*rmquote(char *input, t_data *data);
void					handlesignal(t_data *data);
t_command				*parse_tokens(t_tokens *tokens);
void					free_command_list(t_command *head);
void					print_command_list(t_command *cmd_list);
int						checksyntaxerror(t_data *data);

/***** for ENVP *****/
void					concatenvtoken(t_data *data);
void					init_copy_envp(t_env **head, char **envp);
void					free_list(t_env *head);
void					free_tab(char **tab);
void					append_node(t_env **head, char *key, char *value);
t_env					*create_node(char *key, char *value);
void					print_list(t_env *head);
char					**ft_split_env(char *str, char del);
char					*envvaluestr(char *key, t_data *data);
char					*concatenv(char *input, t_data *data);
int						numchar(char *input, char c);
#endif
