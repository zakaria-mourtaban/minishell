/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 06:10:13 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 22:26:44 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_envp_node(char *key, char *value, int hidden)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->hidden = hidden;
	new_node->next = NULL;
	return (new_node);
}
t_env	*create_envp_list_node(char *envp_str, int hidden)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	ft_split_env(envp_str, &key, &value);
	new_node = create_envp_node(key, value, hidden);
	free(key);
	free(value);
	return (new_node);
}
void	add_node_to_envp_list(t_env **head, t_env **current, t_env *new_node)
{
	if (!*head)
		*head = new_node;
	else
		(*current)->next = new_node;
	*current = new_node;
}

t_env	*init_copy_envp(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	char	*UID;
	pid_t	uid;

	uid = ft_getuid();
	UID = ft_itoa((int)uid);
	head = NULL;
	current = NULL;
	while (*envp)
	{
		new_node = create_envp_list_node(*envp, 0);
		if (!new_node)
			free_env_list(head);
		add_node_to_envp_list(&head, &current, new_node);
		envp++;
	}
	if (set_env(&head, "UID", UID, 0) != 0)
	{
		free_env_list(head);
		return (NULL);
	}
	free(UID);
	return (head);
}

int	set_env(t_env **head, const char *key, const char *value, int hidden)
{
	t_env	*current;
	t_env	*prev;
	t_env	*new_node;

	current = *head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			current->hidden = hidden;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	new_node = create_envp_node((char *)key, (char *)value, hidden);
	printf("newnodehidden:%d\n", new_node->hidden);
	if (!new_node)
		return (-1);
	prev->next = new_node;
	return (0);
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

int	unset_env(t_env **head, const char *key)
{
	t_env	*current;
	t_env	*previous;

	current = *head;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (-1);
}
void	free_list(t_env *head)
{
	t_env	*current;

	while (head)
	{
		current = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = current;
	}
}

void	print_list(t_env *env_head)
{
	t_env	*current_env;
	char	*current_value;

	current_env = env_head;
	while (current_env)
	{
		printf("Key: %s\n", current_env->key);
		current_value = current_env->value;
		printf("Values: ");
		printf("%s", current_value);
		printf("\n\n");
		current_env = current_env->next;
	}
}
