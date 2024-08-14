/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/14 12:10:23 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	signalint;

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	art();
	using_history();
	data.env_list = NULL;
	init_copy_envp(&(data.env_list), env);
	// print list for testing don't forget to remove it from .h file
	// print_list(data.env_list);
	tokenizer(concatenv(ft_strdup("test $ $HOME \"$HOME $HOME\" '$HOME' test | < >> << >"),
			&data), &data);
	remove_quotes(data.cmdchain);
	printcmds(&data);
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
		noninteractivemode(&data, &input);
		initcmd(input, env, &data);
		free(input);
	}
	free_data(&data);
	(void)ac;
	(void)av;
	return (0);
}
