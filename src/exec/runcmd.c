

#include "../../includes/minishell.h"


// runs cmd without any handling
void	runcmd(const char *input, char **env, t_data *data)
{
	pid_t pid;
	int status;

	data->cmd.running = 1;
	data->cmd.cmd = ft_split(input, ' ');
	pid = fork();
	if (pid == -1)
		printf("error in fork should be doing something here");
	if (pid == 0)
	{
		status = execve(get_path(data->cmd.cmd[0], env), data->cmd.cmd, env);
		if (status != 0)
		{
			printf("%s: command not found\n", data->cmd.cmd[0]);
			exit(status);
		}
		exit(status);
	}
	else
		data->cmd.pid = pid;
}
// handles cmd and executes,
// can be modified for future implementation of signals

// void	parsecmd(const char *input, t_data *data)
// {
// 	char *tokens;
// 	tokens = rmquote((char *)input, data);
// }cd , echo , unset , exit

void	initcmd(char *input, char **env, t_data *data)
{
	//check quote closed
	tokenizer(input, data);
	concatenvtoken(data);
	parse_tokens(data->cmdchain);
	if (checksyntaxerror(data))
	{
		printf("error\n");
		//compile as commands and open files
		//execution, basically pipex
	}
	(void)env;
}
