/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:07:10 by zmourtab          #+#    #+#             */
/*   Updated: 2024/09/05 02:27:58 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tokenizer(char *input, t_data *data)
{
	t_tk	tk;

	inittk(input, &tk, data);
	while (input[tk.i] != '\0')
	{
		if (tk.quote)
			tk_quote(input, &tk, data);
		else if (input[tk.i] == '\'' || input[tk.i] == '\"')
			tk_isquote(input, &tk, data);
		else if (ft_strchr("|<> ", input[tk.i]))
			tk_redir(input, &tk, data);
		else
			tk_default(input, &tk, data);
		if (input[tk.i] == '|')
			tk.foundcmd = 0;
		tk.i++;
	}
	tk_bufposo(input, &tk, data);
	free(tk.buffer);
	free(input);
}
