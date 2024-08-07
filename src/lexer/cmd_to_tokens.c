/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_to_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 04:56:29 by afarachi          #+#    #+#             */
/*   Updated: 2024/08/07 15:09:14 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//The lexer is responsible for breaking down the input string into tokens 
//that can be further processed by the parser. 

#include "minishell.h"

t_list_tokens *add_token(t_list_tokens **tokens, t_tokens_type type, char *value)
{
    t_list_tokens *new_token = malloc(sizeof(t_list_tokens));
    new_token->type = type;
    new_token->value = value;
    new_token->next = NULL;

    if (*tokens == NULL)
        *tokens = new_token;
	else
	{
        t_list_tokens *tmp = *tokens;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
    return (new_token);
}

char *collect_word(char **input)
{
    char *start = *input;
    size_t len = 0;

    while (**input && !ft_iswhitespace(**input) && **input != '|'
		&& **input != '<' && **input != '>')
	{
        if (**input == '\'' || **input == '"')
        {
            char quote = *(*input)++;
            while (**input && **input != quote)
                (*input)++;
            if (**input == quote)
                (*input)++;
        } else
            (*input)++;
        len++;
    }
    return (strndup(start, len));
}

// Tokenizer function
int *tokenize(char *input)
{
	t_list_tokens *tokens;
    while (*input) {
        while (ft_iswhitespace(*input))
			input++;
        if (*input == '|')
        {
            add_token(&tokens, TOKEN_PIPE, "|");
            input++;
        } 
        else if (*input == '<' && *(input + 1) == '<')
        {
            add_token(&tokens, TOKEN_HEREDOC, "<<");
            input += 2;
        }
        else if (*input == '<') {
            add_token(&tokens, TOKEN_REDIRECT_IN, "<");
            input++;
        }
        else if (*input == '>' && *(input + 1) == '>')
        {
            add_token(&tokens, TOKEN_APPEND, ">>");
            input += 2;
        }
        else if (*input == '>') {
            add_token(&tokens, TOKEN_REDIRECT_OUT, ">");
            input++;
        } 
        else
        {
            char *word = collect_word(&input);
            add_token(&tokens, TOKEN_WORD, word);
            free(word);
        }
    }
    add_token(&tokens, TOKEN_EOF, NULL);  // End of input
    return tokens;
}

void free_tokens(t_list_tokens *tokens)
{
    while (tokens)
    {
        t_list_tokens *next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}
