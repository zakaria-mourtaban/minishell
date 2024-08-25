/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkraytem <mkraytem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/23 00:15:37 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	volatile signalint;
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	signalint = 0;
	data.cmd.running = 0;
	art();
	using_history();
	data.env_list = NULL;
	data.cmdchain = NULL;
	data.env = env;
	data.env_list = init_copy_envp(env);
	// print_list(data.env_list);
	// printf("#############\n");
	// Initial test input to tokenize, parse, and run
	// tokenizer(concatenv(ft_strdup("test $123 $ $ $ \"$HOME $HOME\" \"'$HOME'\" test | < >> << >"),&data),
	// &data);
	// tokenizer(concatenv(handle_dollar_sign(ft_strdup("test $123 $ $ $ \"$HOME $HOME\" \"'$HOME'\" test | < >> << >"),
	// copy_env),&data),&data);
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
		{
			add_history(input);
			// printf("%s\n", input);
		}
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
