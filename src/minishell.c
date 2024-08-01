/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/01 11:30:49 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_command(const char *input)
{
	if (input == NULL || *input == '\0')
		return ;
	if (ft_strncmp(input, "exit", 10) == 0)
	{
		printf("Exiting...\n");
		exit(0);
	}
	else
		printf("You entered: %s\n", input);
}

int	main(void)
{
	char *input;
	art();

	// Readline setup
	using_history();

	while (1)
	{
		// Display the prompt and read input
		input = readline(">>>");

		if (input == NULL)
		{
			// Handle EOF (Ctrl+D)
			printf("\nExiting...\n");
			break ;
		}
		// Add input to history
		add_history(input);
		// Process the input
		process_command(input);
		free(input);
	}
	return (0);
}