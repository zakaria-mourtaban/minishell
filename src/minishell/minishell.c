/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/16 16:25:25 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	signalint;
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	data.cmd.running = 0;
	art();
	using_history();
	data.env_list = NULL;
	init_copy_envp(&(data.env_list), env);
	// Initial test input to tokenize, parse, and run
	// tokenizer(concatenv(ft_strdup("test $ $HOME \"$HOME $HOME\" '$HOME' test | < >> << >"),&data),
	//&data);
	// remove_quotes(data.cmdchain);
	// printcmds(&data);
	// Parse the tokens into command structures
	// You can now print or execute commands here for debugging
	while (1)
	{
		interactivemode(&data, &input);
		if (input == NULL)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(input) != 0)
			add_history(input);
		// Non-interactive mode processing
		noninteractivemode(&data, &input);
		initcmd(input, env, &data);
	}
	// Clean up before exiting
	free_data(&data);
	(void)ac;
	(void)av;
	return (0);
}
