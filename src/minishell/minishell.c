/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/08/31 23:02:35 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	signalint;
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	signalint = 0;
	art();
	using_history();
	data.env_list = NULL;
	data.cmdchain = NULL;
	data.env = env;
	data.cmd.status = 0;
	data.cmd.running = 0;
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
		// if (data.cmdchain && ft_strcmp(data.cmdchain->content, "echo") == 0)
        //  {
        //      char **args = tokens_to_args(data.cmdchain);
        //      echo_command(args);
        //      free_args(args);
        //  }
        //  else if (data.cmdchain && ft_strcmp(data.cmdchain->content,"env") == 0)
        //  {
        //      char **args = tokens_to_args(data.cmdchain);
        //      env_command(data.env_list);
        //      free_args(args);
        //  }
        //  else if (data.cmdchain && ft_strcmp(data.cmd.cmd[1], "export") == 0)
        //  {
        //      char **args = tokens_to_args(data.cmdchain);
        //      export_command(&data.env_list, args);
        //      free_args(args);
        //  }
        //  else if (data.cmdchain && ft_strcmp(data.cmdchain->content, "pwd") == 0)
        //  {
        //      char **args = tokens_to_args(data.cmdchain);
        //      pwd_command();
        //      free_args(args);
        //  } else if (data.cmdchain && ft_strcmp(data.cmdchain->content, "exit") == 0)
        //  {
        //      char **args = tokens_to_args(data.cmdchain);
        //      exit_command(args);
        //      free_args(args);
        //  }
        //  else if (tokens && strcmp(tokens->value, "unset") == 0)
        //  {
        //      char **args = tokens_to_args(tokens);
        //      unset_env(&envp_list, args[1]);
        //      free_args(args);
        //  }
        //  else if (tokens && strcmp(tokens->value, "cd") == 0)
        //  {
        //      char **args = tokens_to_args(tokens);
        //      change_dir(args, envp_list);
        //      free_args(args);
        //  }
	}
	// Clean up before exiting
	free_env_list(data.env_list);
	(void)ac;
	(void)av;
	return (0);
}
