/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execveutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:10:31 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/05 02:38:59 by odib             ###   ########.fr       */
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
		if (exec && access(exec, F_OK | X_OK) == 0)
			return (ft_free_tab(allpath), ft_free_tab(s_cmd), exec);
		if (exec)
			free(exec);
	}
	return (ft_free_tab(allpath), ft_free_tab(s_cmd), ft_strdup(cmd));
}

void	execute_pipeline(t_command *cmds, t_data *data)
{
	t_exp	xp;

	expp1(&xp, cmds, data);
	if (!xp.pipes)
		return ;
	xp.i = 0;
	if (expp5(&xp, data))
		return ;
	xp.i = 0;
	xp.current = cmds;
	while (xp.current)
	{
		if (ft_strlen(xp.current->args->arg) == 0)
		{
			xp.current = xp.current->next;
			continue ;
		}
		expp2(&xp, data);
		expp3(&xp, data);
	}
	expp4(&xp, data);
}
