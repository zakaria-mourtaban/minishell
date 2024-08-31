/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:24:52 by odib              #+#    #+#             */
/*   Updated: 2024/08/31 22:50:15 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;
	int		temphidden;

	temp_key = a->key;
	temp_value = a->value;
	temphidden = a->hidden;
	a->hidden = b->hidden;
	a->key = b->key;
	a->value = b->value;
	b->hidden = temphidden;
	b->key = temp_key;
	b->value = temp_value;
}

void	sort_env_list(t_env *head)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;

	lptr = NULL;
	if (head == NULL)
		return ;
	do
	{
		swapped = 0;
		ptr1 = head;
		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->key, ptr1->next->key) > 0)
			{
				swap_nodes(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

void	remove_quotes_from_str(char *str)
{
	char	*src;
	char	*dst;
	char	quote;

	quote = 0;
	src = str;
	dst = str;
	while (*src)
	{
		if (!quote && (*src == '\'' || *src == '\"'))
			quote = *src;
		else if (quote && *src == quote)
			quote = 0;
		else
		{
			*dst = *src;
			dst++;
		}
		src++;
	}
	*dst = '\0';
}

int	check_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		return (0);
	}
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_sorted_env_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		printf("%s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

int	is_input_null(char **input)
{
	return (input[1] == NULL);
}

int	is_key_invalid(char *key)
{
	if (check_key(key) == 0)
	{
		printf("Invalid key: %s\n", key); // Debugging
		return (1);
	}
	return (0);
}

void	free_resources(char *key, char *value)
{
	free(value);
	free(key);
}

char	**argtochar(t_arg *arg)
{
	t_arg	*tmp;
	char	**args;
	int		i;

	i = 0;
	tmp = arg;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	args = (char **)malloc((i + 1) * sizeof(char *));
	if (args == NULL)
		return (NULL);
	tmp = arg;
	i = 0;
	while (tmp != NULL)
	{
		args[i] = ft_strdup(tmp->arg);
		printf("args[i]:%s\n", args[i]);
		tmp = tmp->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}
int	getac(t_arg *arg)
{
	t_arg	*tmp;
	int		i;

	tmp = arg;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

void	export_command(t_env **env_list, t_arg *arg)
{
	char	*key;
	char	*value;
	t_arg	*tmparg;
	int		i;

	tmparg = arg;
	i = 0;
	tmparg = tmparg->next;
	while (tmparg)
	{
		split_envp(tmparg->arg, &key, &value);
		if (is_key_invalid(key))
		{
			free_resources(key, value);
			tmparg = tmparg->next;
			continue ;
		}
		if (ft_strlen(value) == 0)
		{
			printf("setting env\n");
			set_env(env_list, key, value, 1);
		}
		else
		{
			set_env(env_list, key, value, 0);
		}
		free_resources(key, value);
		tmparg = tmparg->next;
		i++;
	}
	sort_env_list(*env_list);
	if (i == 0)
		print_sorted_env_list(*env_list);
}

// int	update_env_value(t_env *head, const char *key, const char *new_value)
//{
//	t_env	*current;

//	current = head;
//	// Traverse the list to find the node with the matching key
//	while (current)
//	{
//		if (strcmp(current->key, key) == 0) // If the key matches
//		{
//			free(current->value);               // Free the old value
//			current->value = strdup(new_value); // Update with the new value
//			if (!current->value)
//				return (-1); // Memory allocation failure
//			return (0);      // Success
//		}
//		current = current->next;
//	}
//	return (-1); // Key not found
//}