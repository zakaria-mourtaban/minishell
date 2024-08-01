/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/01 23:31:13 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	cmd;

void	handle_command(const char *input, char **env)
{
	pid_t	pid;

	cmd.cmd = ft_split(input, ' ');
	pid = fork();
	if (pid == -1)
		printf("error in fork should be doing something here");
	if (pid == 0)
		execve(get_path(cmd.cmd[0], env), cmd.cmd, env);
	else
		waitpid(pid,&cmd.status, 0);
}

void	process_command(const char *input, char **env)
{
	if (input == NULL || *input == '\0')
		return ;
	if (ft_strncmp(input, "exit", 10) == 0)
	{
		printf("Exiting...\n");
		exit(0);
	}
	else
		handle_command(input, env);
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	art();
	using_history();
	while (1)
	{
		input = readline(">>>");
		if (input == NULL)
		{
			printf("\nExiting...\n");
			break ;
		}
		add_history(input);
		process_command(input, env);
		free(input);
	}
	(void)ac;
	(void)av;
	return (0);
}
// Display the prompt and read input
// Handle EOF (Ctrl+D)
// Add input to history
// Process the input