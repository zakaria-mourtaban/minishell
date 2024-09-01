/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:15:38 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 17:59:45 by zmourtab         ###   ########.fr       */
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
# include <errno.h>
# include <termios.h>

/* Readline library */
# include <readline/history.h>
# include <readline/readline.h>

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
	TOKEN_SPACE = 9,
	TOKEN_DIRECTORY = 10,
	TOKEN_START = 11,
}						e_token;

typedef struct s_tokens
{
	e_token				id;
	int					error;
	char				*content;
	struct s_tokens		*next;
	struct s_tokens		*previous;
}						t_tokens;

typedef struct s_env
{
	char				*key;
	char				*value;
	int					hidden;
	struct s_env		*next;
}						t_env;

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
	int					append;
	int					error;
	struct s_command	*next;
}						t_command;

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
	char				**env;
}						t_data;

extern int				signalint;
void					check_path(const char *path, t_data *data);
int						hasaccess(t_tokens *token, t_data *data);
void					execute_pipeline(t_command *cmds, t_data *data);
void					art(void);
void					free_cmdchain(t_tokens *cmdchain);
void					free_data(t_data *data);
void					free_command_list(t_command *head);
void					free_list(t_env *head);
void					free_tab(char **tab);
void					free_env_list(t_env *env_list);
void					tokenizer(char *input, t_data *data);
void					printcmds(t_data *data);
char					*get_path(char *cmd, t_env *envp_list);
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
void					print_command_list(t_command *cmd_list);
int						checksyntaxerror(t_data *data);
void					remove_quotes(t_tokens *tokens);
void					specify_token_cmd(t_tokens *token);
char					*get_env(t_env *head, const char *key);
char					*get_env_copy(char *name, t_env *copy_env);
void					replace_env(char **input, char *old, char *new);
pid_t					ft_getpid(void);
char					*handle_double_dollar(char *input);
char					*remove_char(char *str, char char_to_remove);
int						ft_is_delimiter(char c);

char					*handle_dollar_sign(char *input, t_data *data);

/***** for ENVP *****/
void					concatenvtoken(t_data *data);
t_env					*init_copy_envp(char **envp);

t_env					*create_node(char *key, char *value);
t_env					*create_envp_list_node(char *envp_str, int hidden);
t_env					*create_envp_node(char *key, char *value, int hidden);
void					print_list(t_env *head);

char					*envvaluestr(char *key, t_data *data);
char					*concatenv(char *input, t_data *data);
int						numchar(char *input, char c);

void					print_sorted_env_list(t_env *head);
char					*get_env(t_env *head, const char *key);
int						set_env(t_env **head, const char *key,
							const char *value, int hidden);
void					ft_split_env(char *envp_str, char **key, char **value);
int						unset_env(t_env **head, const char *key);
void					print_list(t_env *env_head);
pid_t					ft_getuid(void);

bool					is_builtin_command(const char *command);
void					pwd_command(void);
void					exit_command(t_arg *args);
void					echo_command(t_arg *args);
void					env_command(t_env *env_list);
void					export_command(t_env **env_list, t_arg *arg);
void					split_envp(char *envp_str, char **key, char **value);
int						ft_strlen1(const char *s);
char					*ft_strndup(const char *s, size_t n);
char					*ft_strncpy(char *dest, const char *src, size_t n);
size_t					ft_strnlen(const char *str, size_t n);
int						change_dir(t_arg *args, t_env *env_list);
char					**tokens_to_args(char *tokens);
void					free_args(char **args);
int						unset_command(t_arg *args, t_env **env_list);
#endif
