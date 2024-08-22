/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:43:26 by odib              #+#    #+#             */
/*   Updated: 2024/08/21 04:11:33 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_node(char *key, char *value)
{
	t_env	*res;

	res = malloc(sizeof(t_env));
	if (!res)
	{
		perror("Failed to allocate memory for new node");
		return (NULL);
	}
	res->key = ft_strdup(key);
	res->value = ft_strdup(value);
	res->next = NULL;
	return (res);
}

void	append_node(t_env **head, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = create_node(key, value);
	if (!new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_list(t_env *env_head)
{
	t_env	*current_env;
	t_env	*next_env;

	current_env = env_head;
	while (current_env)
	{
		next_env = current_env->next;
		// Free the key
		free(current_env->key);
		// Free the value list associated with this env node
		free(current_env->value);
		// Free the env node itself
		free(current_env);
		current_env = next_env;
	}
}

void	init_copy_envp(t_env **head, char **envp)
{
	char	**copy_env;
	int		i;

	i = 0;
	while (envp[i] != NULL)
	{
		copy_env = ft_split_env(envp[i], '=');
		if (copy_env)
		{
			if (copy_env[0] && copy_env[1])
				append_node(head, copy_env[0], copy_env[1]);
			else if (copy_env[0] && !copy_env[1])
				append_node(head, copy_env[0], ft_strdup(""));
			free_tab(copy_env);
		}
		i++;
	}
}
// for testing

void	print_list(t_env *env_head)
{
	t_env	*current_env;
	char	*current_value;

	current_env = env_head;
	while (current_env)
	{
		// Print the key
		printf("Key: %s\n", current_env->key);
		// Print the values
		current_value = current_env->value;
		printf("Values: ");
		printf("%s", current_value);
		printf("\n\n"); // Print a newline between entries
		// Move to the next env node
		current_env = current_env->next;
	}
}

// int count_envp_elements(char **envp)
// {
// 	int count = 0;
// 	while (envp[count] != NULL)
// 	{
// 		count++;
// 	}
// 	return (count);
// }

// int list_length(t_env *head)
// {
// 	int length = 0;
// 	t_env *current = head;
// 	while (current)
// 	{
// 		length++;
// 		current = current->next;
// 	}
// 	return (length);
// }