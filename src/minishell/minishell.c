/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:22:42 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/05 20:36:40 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_signalint;

char	*envtostr(t_env *envs)
{
	char	*out;

	out = ft_strdup(envs->key);
	out = ft_strjoingnl(out, "=");
	if (envs->value != NULL)
		out = ft_strjoingnl(out, envs->value);
	return (out);
}

char	**createenv(t_env *env)
{
	char	**out;
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	out = malloc((i + 1) * sizeof(char *));
	tmp = env;
	i = 0;
	while (tmp)
	{
		out[i] = envtostr(tmp);
		tmp = tmp->next;
		i++;
	}
	out[i] = NULL;
	return (out);
}

t_env	*get_tenv(t_env *head, const char *key)
{
	t_env	*tmp;

	tmp = head;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0
			&& ft_strlen(key) == ft_strlen(tmp->key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	init(t_data *data, char **env)
{
	t_env	*tmp;
	char	*tmpval;

	art();
	g_signalint = 0;
	data->env_list = NULL;
	data->cmdchain = NULL;
	data->env = NULL;
	data->cmd.status = 0;
	data->cmd.running = 0;
	data->tmpfd = -1;
	data->env_list = init_copy_envp(env);
	tmp = get_tenv(data->env_list, "SHLVL");
	if (tmp == NULL)
		set_env(&data->env_list, "SHLVL", "1", 0);
	else
	{
		tmpval = ft_strdup(tmp->value);
		free(tmp->value);
		tmp->value = ft_itoa(ft_atoi(tmpval) + 1);
		free(tmpval);
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_data	data;

	init(&data, env);
	while (1)
	{
		if (data.env != NULL)
			free_tab(data.env);
		data.env = createenv(data.env_list);
		interactivemode(&data, &input);
		if (input == NULL)
		{
			printf("\n");
			break ;
		}
		if (ft_strlen(input) != 0 && input[0] != ' ')
			add_history(input);
		noninteractivemode(&data, &input);
		initcmd(input, env, &data);
	}
	free_env_list(data.env_list);
	free_args(data.env);
	return (0);
	(void)ac;
	(void)av;
}
