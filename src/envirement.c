/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:43:26 by odib              #+#    #+#             */
/*   Updated: 2024/08/08 23:43:48 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env *create_node(char *key, char *value)
{
	t_env *res = malloc(sizeof(t_env));
	if (!res)
	{
		perror("Failed to allocate memory for new node");
		return NULL;
	}
	res->key = ft_strdup(key);
	res->value = ft_strdup(value);
	res->next = NULL;
	if (!res->key || !res->value)
	{
		perror("Failed to allocate memory for key or value");
		free(res->key);
		free(res->value);
		free(res);
		return NULL;
	}
	return res;
}

void append_node(t_env **head, char *key, char *value)
{
	t_env *new_node = create_node(key, value);
	if (!new_node)
		return;

	if (*head == NULL)
		*head = new_node;
	else
	{
		t_env *current = *head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void free_tab(char **tab)
{
	int i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void free_list(t_env *head)
{
	t_env *temp;
	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void init_copy_envp(t_env **head, char **envp)
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
				append_node(head, copy_env[0], "");
			free_tab(copy_env);
		}
		i++;
	}
}

// for testing
//
void print_list(t_env *head)
{
	t_env *current = head;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

// int count_envp_elements(char **envp)
// {
// 	int count = 0;
// 	while (envp[count] != NULL)
// 	{
// 		count++;
// 	}
// 	return count;
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
// 	return length;
// }