/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:24:52 by odib              #+#    #+#             */
/*   Updated: 2024/09/03 00:23:50 by odib             ###   ########.fr       */
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
	swapped = 1;
	while (swapped)
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
	}
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
		printf("Invalid key: %s\n", key);
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

int	is_valid_export_argument(char *arg)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	split_envp(arg, &key, &value);
	if (!key || ft_strlen(key) == 0 || is_key_invalid(key))
	{
		free_resources(key, value);
		return (0);
	}
	free_resources(key, value);
	return (1);
}

int	set_export_variable(t_env **env_list, char *arg)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	split_envp(arg, &key, &value);
	if (!key || ft_strlen(key) == 0 || is_key_invalid(key))
		return (free_resources(key, value), -1);
	if (ft_strlen(value) == 0)
	{
		if (set_env(env_list, key, value, 1) != 0)
			return (free_resources(key, value), -1);
	}
	else
	{
		if (set_env(env_list, key, value, 0) != 0)
			return (free_resources(key, value), -1);
	}
	free_resources(key, value);
	return (0);
}

int	export_command(t_env **env_list, t_arg *arg)
{
	t_arg	*tmparg;
	int		status;

	status = 0;
	tmparg = arg->next;
	if (tmparg == NULL)
	{
		print_sorted_env_list(*env_list);
		return (status);
	}
	while (tmparg)
	{
		if (!is_valid_export_argument(tmparg->arg))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(tmparg->arg, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
			break ;
		}
		if (set_export_variable(env_list, tmparg->arg) != 0)
			status = 1;
		tmparg = tmparg->next;
	}
	return (status);
}

// done
// int	export_command(t_env **env_list, t_arg *arg)
// {
// 	char	*key;
// 	char	*value;
// 	t_arg	*tmparg;
// 	int		status;

// 	key = NULL;
// 	value = NULL;
// 	status = 0;
// 	tmparg = arg->next;

// 	if (tmparg == NULL)
// 		return (print_sorted_env_list(*env_list),status);
// 	while (tmparg)
// 	{
// 		split_envp(tmparg->arg, &key, &value);
// 		if (!key || ft_strlen(key) == 0)
// 		{
// 			ft_putstr_fd("export: `", STDERR_FILENO);
// 			ft_putstr_fd(tmparg->arg, STDERR_FILENO);
// 			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
// 			free_resources(key, value);
// 			status = 1;
// 			break;
// 		}
// 		if (is_key_invalid(key))
// 		{
// 			free_resources(key, value);
// 			status = 1;
// 			tmparg = tmparg->next;
// 			continue;
// 		}
// 		if (ft_strlen(value) == 0)
// 		{
// 			if (set_env(env_list, key, value, 1) != 0)
// 				status = 1;
// 		}
// 		else
// 			if (set_env(env_list, key, value, 0) != 0)
// 				status = 1;
// 		free_resources(key, value);
// 		tmparg = tmparg->next;
// 	}
// 	return (status);
// }


// int	export_command(t_env **env_list, t_arg *arg)
// {
// 	char	*key;
// 	char	*value;
// 	t_arg	*tmparg;
// 	int		i;
// 	int		status;

// 	key = NULL;
// 	value = NULL;
// 	i = 0;
// 	status = 0;
// 	tmparg = arg->next;
// 	while (tmparg)
// 	{
// 		split_envp(tmparg->arg, &key, &value);

// 		// Check if the key is missing or invalid
// 		if (!key || ft_strlen(key) == 0)
// 		{
// 			ft_putstr_fd("export: `", STDERR_FILENO);
// 			ft_putstr_fd(tmparg->arg, STDERR_FILENO);
// 			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
// 			free_resources(key, value);
// 			status = 1;
// 			tmparg = tmparg->next;
// 			break ;
// 		}

// 		if (is_key_invalid(key))
// 		{
// 			free_resources(key, value);
// 			status = 1;
// 			tmparg = tmparg->next;
// 			continue ;
// 		}

// 		if (ft_strlen(value) == 0)
// 		{
// 			if (set_env(env_list, key, value, 1) != 0)
// 			{
// 				status = 1;
// 			}
// 		}
// 		else
// 		{
// 			if (set_env(env_list, key, value, 0) != 0)
// 			{
// 				status = 1;
// 			}
// 		}

// 		free_resources(key, value);
// 		tmparg = tmparg->next;
// 		i++;
// 	}
// 	if (key || ft_strlen(key) != 0)
// 	{
// 		sort_env_list(*env_list);
// 	}
// 	if (i == 0 && ft_strlen(key) != 0)
// 	{
// 		print_sorted_env_list(*env_list);
// 	}

// 	return (status);
// }


// int	export_command(t_env **env_list, t_arg *arg)
// {
// 	char	*key;
// 	char	*value;
// 	t_arg	*tmparg;
// 	int		i;
// 	int		status;

// 	key = NULL;
// 	value = NULL;
// 	i = 0;
// 	status = 0;
// 	tmparg = arg->next;
// 	while (tmparg)
// 	{
// 		split_envp(tmparg->arg, &key, &value);
// 		if (is_key_invalid(key))
// 		{
// 			free_resources(key, value);
// 			status = 1;
// 			tmparg = tmparg->next;
// 			continue ;
// 		}
// 		if (ft_strlen(value) == 0)
// 		{
// 			if (set_env(env_list, key, value, 1) != 0)
// 			{
// 				status = 1;
// 			}
// 		}
// 		else
// 		{
// 			if (set_env(env_list, key, value, 0) != 0)
// 			{
// 				status = 1;
// 			}
// 		}
// 		free_resources(key, value);
// 		tmparg = tmparg->next;
// 		i++;
// 	}
// 	sort_env_list(*env_list);
// 	if (i == 0)
// 	{
// 		print_sorted_env_list(*env_list);
// 	}
// 	return (status);
// }