/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execveutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:10:31 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/01 11:12:25 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// char	*my_getenv(char *name, char **env)
// {
// 	int		i;
// 	int		j;
// 	char	*sub;

// 	i = 0;
// 	while (env[i])
// 	{
// 		j = 0;
// 		while (env[i][j] && env[i][j] != '=')
// 			j++;
// 		sub = ft_substr(env[i], 0, j);
// 		if (ft_strcmp(sub, name) == 0)
// 		{
// 			free(sub);
// 			return (env[i] + j + 1);
// 		}
// 		free(sub);
// 		i++;
// 	}
// 	return (NULL);
// }
char	*get_env(t_env *head, const char *key)
{
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0
			&& ft_strlen(key) == ft_strlen(head->key))
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (ft_strdup(""));
}

char	*get_path(char *cmd, t_env *envp_list)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	i = -1;
	path_part = get_env(envp_list, "PATH");
	allpath = ft_split(path_part, ':');
	free(path_part);
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(allpath);
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (ft_strdup(cmd));
}
