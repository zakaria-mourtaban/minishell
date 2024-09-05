/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 22:15:38 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/05 20:25:13 by zmourtab         ###   ########.fr       */
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

/* ascii art */
# include "art.h"

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
	TOKEN_FILE = 10,
	TOKEN_DIRECTORY = 11,
	TOKEN_START = 12,
}						t_token;

typedef struct s_tokens
{
	t_token				id;
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

typedef struct s_tk
{
	int					i;
	char				*buffer;
	int					buf_i;
	char				quote;
	int					last_was_space;
	int					foundcmd;
}						t_tk;

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
	int					tmpfd;
}						t_data;

typedef struct s_ex
{
	char				**args;
	int					arg_count;
	t_arg				*current;
	pid_t				pid;
	char				*path;
	int					j;
	t_command			*cmd;
	int					*pipes;
	int					i;
	int					num_cmds;
	t_data				*data;
}						t_ex;

extern int				g_signalint;
// t_arg					*create_arg_node(char *arg);
// void					add_argument(t_command *cmd, char *arg);
// t_command				*create_command_node(void);
// void					append_command_node(t_command **cmd_list,
// 							t_command *new_cmd);
// void					free_tab(char **tab);
// char					*handle_dollar_sign(char *input, t_data *data);
// int						check_path(t_tokens *tokens, t_data *data);
// int						hasaccess(t_tokens *token, t_data *data);
// void					execute_pipeline(t_command *cmds, t_data *data);
// void					free_cmdchain(t_tokens *cmdchain);
// void					free_data(t_data *data);
// void					free_command_list(t_command *head);
// void					free_list(t_env *head);
// void					free_tab(char **tab);
// void					free_env_list(t_env *env_list);
// void					tokenizer(char *input, t_data *data);
// void					printcmds(t_data *data);
// char					*get_path(char *cmd, t_env *envp_list);
// void					runcmd(const char *input, char **env, t_data *data);
// void					noninteractivehandle_sigquit(int sig);
// void					noninteractivehandle_sigint(int sig);
// void					interactivehandle_sigquit(int sig);
// void					interactivehandle_sigint(int sig);
// void					interactivemode(t_data *data, char **input);
// void					noninteractivemode(t_data *data, char **input);
// char					*rmquote(char *input, t_data *data);
// void					handlesignal(t_data *data);
// t_command				*parse_tokens(t_tokens *tokens);
// void					print_command_list(t_command *cmd_list);
// int						checksyntaxerror(t_data *data);
// void					specify_token_cmd(t_tokens *token);
// char					*get_env(t_env *head, const char *key);
// char					*get_env_copy(char *name, t_env *copy_env);
// void					replace_env(char **input, char *old, char *new);
// pid_t					ft_getpid(void);
// char					*handle_double_dollar(char *input);
// char					*remove_char(char *str, char char_to_remove);
// int						ft_is_delimiter(char c);

// char					*handle_dollar_sign(char *input, t_data *data);

// /***** for ENVP *****/
// t_env					*init_copy_envp(char **envp);

// t_env					*create_node(char *key, char *value);
// t_env					*create_envp_list_node(char *envp_str, int hidden);
// t_env					*create_envp_node(char *key, char *value,
// int hidden);
// void					print_list(t_env *head);

// char					*envvaluestr(char *key, t_data *data);
// char					*concatenv(char *input, t_data *data);
// int						numchar(char *input, char c);

// char					*get_env(t_env *head, const char *key);
// int						set_env(t_env **head, const char *key,
// 							const char *value, int hidden);
// int						unset_env(t_env **head, const char *key);
// void					print_list(t_env *env_head);
// pid_t					ft_getuid(void);

// int						is_builtin_command(const char *command);
// char					*concatenvloop(char *input, t_data *data);
// int						is_input_null(char **input);
// char					**tokens_to_args(char *tokens);
// char					**createenv(t_env *env);
// void					swap_nodes(t_env *a, t_env *b);
// void					sort_env_list(t_env *head);
// int						dollarcount(char *input);
// int	execute_builtin_command_nofork(t_command *command, t_data *data);
// void	execute_command(t_ex *ex);
// void	close_pipes(int *pipes, int num_pipes);

void					ft_split_env(char *envp_str, char **key, char **value);
int						pwd_command(void);
int						exit_command(t_arg *args);
void					print_sorted_env_list(t_env *head);
void					concatenvtoken(t_data *data);
void					initcmd(char *input, char **env, t_data *data);
void					art(void);
int						echo_command(t_arg *args);
int						env_command(t_env *env_list);
int						export_command(t_env **env_list, t_arg *arg);
void					split_envp(char *envp_str, char **key, char **value);
int						ft_strlen1(const char *s);
char					*ft_strndup(const char *s, size_t n);
char					*ft_strncpy(char *dest, const char *src, size_t n);
size_t					ft_strnlen(const char *str, size_t n);
int						change_dir(t_arg *args, t_env *env_list);
void					free_args(char **args);
int						unset_command(t_arg *args, t_env **env_list);
void					free_list_arg(t_arg *head);
void					free_resources(char *key, char *value);
int						is_key_invalid(char *key);
void					remove_quotes_from_str(char *str);
int						check_key(char *key);
void					print_sorted_env_list(t_env *head);

/*		signals		*/
void					noninteractivehandle_sigquit(int sig);
void					noninteractivehandle_sigint(int sig);
void					interactivehandle_sigquit(int sig);
void					interactivehandle_sigint(int sig);
void					handlesignal(t_data *data);
char					*getinfo(t_data *data);
void					interactivemode(t_data *data, char **input);
void					noninteractivemode(t_data *data, char **input);

/*		utils		*/
void					remove_quotes(t_tokens *tokens);
void					free_data(t_data *data);
void					free_env_list(t_env *env_list);
void					free_cmdchain(t_tokens *cmdchain);
void					free_command_list(t_command *head);
void					free_arg_list(t_arg *head);
void					printerror(t_tokens *token);
int						checkheredoc(t_tokens *token);
t_tokens				*getnextcommand(t_tokens *tmp);

/*		env		*/

typedef struct s_isexpandable
{
	int					i;
	int					sinquote;
	int					doublequote;
	int					isexpandable;
}						t_isexpandable;

char					*concatenvloop(char *input, t_data *data);
char					*envvaluestr(char *key, t_data *data);
int						find_char_index(char *string, char c);
char					*concatenv(char *input, t_data *data);
int						dollarcount(char *input);
char					*read_pid_line(int fd);
ssize_t					read_status_file(char *buffer, size_t size);
pid_t					ft_getpid(void);
pid_t					ft_getuid(void);
t_env					*create_envp_node(char *key, char *value, int hidden);
t_env					*create_envp_list_node(char *envp_str, int hidden);
void					add_node_to_envp_list(t_env **head, t_env **current,
							t_env *new_node);
t_env					*init_copy_envp(char **envp);
int						set_env(t_env **head, const char *key,
							const char *value, int hidden);
void					free_tab(char **tab);
void					free_list(t_env *head);
char					*handle_dollar_sign(char *input, t_data *data);
void					copy_and_append_char(char **result, char *input,
							int *i);
void					handle_question_mark(char **result, int *i,
							t_data *data);
void					handle_two_dollar(char **result, int *i);
void					handle_normal_variable(char *input, int *i,
							char **result, t_env *env);
void					split_envp(char *envp_str, char **key, char **value);
int						isquote(char c);

/*		parsing		*/
t_arg					*create_arg_node(char *arg);
void					add_argument(t_command *cmd, char *arg);
t_command				*create_command_node(void);
t_command				*create_command_node(void);
void					append_command_node(t_command **cmd_list,
							t_command *new_cmd);
int						hasaccess(t_tokens *token, t_data *data);
t_tokens				*nexttoken(t_tokens *tokens);
t_tokens				*getnextspace(t_tokens *token);
void					handle_word_command_token(t_tokens *tmp,
							t_command **current_cmd);
void					handle_pipe_token(t_command **cmd_list,
							t_command **current_cmd);
t_tokens				*handle_in_file_token(t_tokens *tmp,
							t_command *current_cmd);
t_tokens				*handle_out_file_token(t_tokens *tmp,
							t_command *current_cmd, int append);
t_command				*initialize_command_list(t_tokens *tokens);
void					process_token(t_tokens **tmp, t_command **cmd_list,
							t_command **current_cmd);
void					process_tokens_loop(t_tokens *tokens,
							t_command **cmd_list, t_command **current_cmd);
t_command				*parse_tokens(t_tokens *tokens);

/*		error check		*/
t_tokens				*getprev(t_tokens *token);
t_tokens				*getnext(t_tokens *token);
int						checkpipe(t_tokens *token);
int						checksyntaxerror(t_data *data);
int						checkfileoutappend(t_tokens *token);
int						checkheredoc(t_tokens *token);
int						checkfileout(t_tokens *token);
int						checkfilein(t_tokens *token);
int						is_command(t_tokens *token);
int						isdirectory(t_tokens *token);
int						contains_dot_or_slash(const char *str);
void					printerror(t_tokens *token);
void					printerrnocmd(t_tokens *token, t_data *data);
void					printerrnodir(t_tokens *token, t_data *data);
void					printerrnofdir(t_tokens *token, t_data *data);
void					checkpipeerr(t_data *data);
int						casenext(t_tokens **tmp);
void					handleerrpipe(t_data *data, t_tokens **tmp,
							t_tokens **tmpprint);

/*		tokenizer		*/

t_tokens				*newnode(char *data, int type);
void					append(t_tokens **cmds, char *data, int type);
void					printcmds(t_data *data);
t_token					get_delimiter_type(char *str);
void					inittk(char *input, t_tk *tk, t_data *data);
void					tk_quote(char *input, t_tk *tk, t_data *data);
void					tk_isquote(char *input, t_tk *tk, t_data *data);
void					tk_bufpos(char *input, t_tk *tk, t_data *data);
int						tk_heredocument(char *input, t_tk *tk, t_data *data);
int						tk_outappend(char *input, t_tk *tk, t_data *data);
void					tk_appendfo(char *input, t_tk *tk, t_data *data);
void					tk_appendfin(char *input, t_tk *tk, t_data *data);
void					tk_redir(char *input, t_tk *tk, t_data *data);
void					tk_default(char *input, t_tk *tk, t_data *data);
void					tk_bufposo(char *input, t_tk *tk, t_data *data);
void					tokenizer(char *input, t_data *data);
void					addnode(t_tokens **tmp, char *data, int type);
t_tokens				*newnode(char *data, int type);

/*		execution		*/
typedef struct s_heredocs
{
	t_tokens			*tmp;
	char				*input;
	char				*tmpstr;
	char				*buffer;
	int					hasquote;
	int					i;
}						t_hd;

typedef struct s_exp
{
	int					*pipes;
	int					num_cmds;
	int					i;
	t_command			*current;
	int					in;
	int					out;
	char				*path;

}						t_exp;

void					free_tokens(t_tokens *head);
void					ft_free_tab(char **tab);
char					*get_env(t_env *head, const char *key);
char					*get_path(char *cmd, t_env *envp_list);
void					expp1(t_exp *xp, t_command *cmds, t_data *data);
void					expp2(t_exp *xp, t_data *data);
void					expp3(t_exp *xp, t_data *data);
void					expp4(t_exp *xp, t_data *data);
int						expp5(t_exp *xp, t_data *data);
void					execute_pipeline(t_command *cmds, t_data *data);
int						check_quotes(const char *str);
void					fixupcommand(t_data *data);
void					fixuptoken(t_data *data);
t_tokens				*getcommandsfirst(t_command **current_cmd,
							t_tokens *tmp);
t_tokens				*handleoutafile(t_data *data, t_command **current_cmd,
							t_tokens *tmp);
t_tokens				*handleoutfile(t_data *data, t_command **current_cmd,
							t_tokens *tmp);
t_tokens				*handleinfile(t_data *data, t_command **current_cmd,
							t_tokens *tmp);
t_command				*getcommands(t_data *data);
void					getcommandssecond(t_command **cmd_list,
							t_command **current_cmd, t_tokens **tmp);
void					writefile(t_hd *hd, t_data *data);
void					infile(t_hd *hd);
void					inheredoc(t_hd *hd);
char					*heredocrmquote(char *input);
void					handleredirects(t_data *data, t_command *command);
void					parseheredoc(t_data *data);
int						haserror(t_tokens *token);
int						checkheredocerror(t_data *data);
void					handleheredoc(t_data *data, t_tokens *token);
int						exececho(t_command *command, t_data *data);
int						execcd(t_command *command, t_data *data);
int						execpwd(t_command *command, t_data *data);
int						execenv(t_command *command, t_data *data);
int						execexit(t_command *command, t_data *data);
void					close_pipes(int *pipes, int num_pipes);
int						is_builtin_command(const char *command);
int						execunset(t_command *command, t_data *data);
int						execexport(t_command *command, t_data *data);
int						execute_builtin_command_nofork(t_command *command,
							t_data *data);
int						exec3(t_command *command, t_data *data);
int						exec4(t_command *command, t_data *data);
int						execute_builtin_command(t_command *command,
							t_data *data);
void					dupexecnofork(t_command *command);
void					ex1(pid_t *pid);
void					ex2(t_ex *ex);
void					ex3(t_ex *ex);
void					ex4(t_ex *ex);
void					execute_command(t_ex *ex);

#endif
