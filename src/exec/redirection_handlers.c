/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 06:07:05 by odib              #+#    #+#             */
/*   Updated: 2024/09/06 06:25:40 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handleredirects(t_data *data, t_command *command)
{
	t_tokens	*tmp;
	t_command	*current_cmd;

	current_cmd = command;
	tmp = data->cmdchain;
	while (current_cmd)
	{
		while (tmp)
		{
			if (tmp->id == TOKEN_IN_FILE && tmp->error == 0)
				tmp = handleinfile(data, &current_cmd, tmp);
			else if (tmp->id == TOKEN_OUT_FILE && tmp->error == 0)
				tmp = handleoutfile(data, &current_cmd, tmp);
			else if (tmp->id == TOKEN_OUT_A_FILE && tmp->error == 0)
				tmp = handleoutafile(data, &current_cmd, tmp);
			if (tmp && tmp->id == TOKEN_PIPE)
				break ;
			if (tmp)
				tmp = tmp->next;
		}
		if (tmp != NULL)
			tmp = tmp->next;
		if (current_cmd != NULL)
			current_cmd = current_cmd->next;
	}
}

char	*heredocrmquote(char *input)
{
	size_t		length;
	const char	*src;
	char		*output;
	char		*dest;

	length = ft_strlen(input);
	output = (char *)malloc(length + 1);
	if (output == NULL)
	{
		perror("Unable to allocate memory");
		exit(EXIT_FAILURE);
	}
	dest = output;
	src = input;
	while (*src != '\0')
	{
		if (*src != '\'')
		{
			*dest = *src;
			dest++;
		}
		src++;
	}
	*dest = '\0';
	return (output);
}

void	inheredoc(t_hd *hd)
{
	hd->hasquote = 0;
	hd->tmp->id = TOKEN_IN_FILE;
	free(hd->tmp->content);
	hd->tmp->content = ft_strdup("<");
	hd->tmp = hd->tmp->next;
	if (hd->tmp && hd->tmp->id == TOKEN_SPACE)
		hd->tmp = hd->tmp->next;
}

void	infile(t_hd *hd)
{
	if (ft_strchr(hd->tmp->content, '\'') != NULL)
		hd->hasquote = 1;
	hd->tmp->content = heredocrmquote(hd->tmp->content);
	hd->input = NULL;
	hd->buffer = NULL;
	while (1)
	{
		hd->input = readline("> ");
		if (hd->input && ft_strcmp(hd->input, hd->tmp->content) == 0
			&& ft_strlen(hd->input) == ft_strlen(hd->tmp->content))
			break ;
		if (hd->input == NULL)
			break ;
		if (hd->buffer == NULL)
			hd->buffer = ft_strdup(hd->input);
		else
			hd->buffer = ft_strjoingnl(hd->buffer, hd->input);
		hd->buffer = ft_strjoingnl(hd->buffer, "\n");
		free(hd->input);
	}
}

void	writefile(t_hd *hd, t_data *data)
{
	if (hd->buffer == NULL)
		hd->buffer = ft_strdup("");
	hd->tmpstr = ft_itoa(hd->i);
	data->tmpfd = open(hd->tmpstr, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (hd->hasquote == 0)
		hd->buffer = handle_dollar_sign(hd->buffer, data);
	if (hd->buffer)
		ft_putstr_fd(hd->buffer, data->tmpfd);
	close(data->tmpfd);
	free(hd->buffer);
	hd->tmp->content = ft_strdup(hd->tmpstr);
	free(hd->tmpstr);
	hd->tmp = hd->tmp->next;
}
