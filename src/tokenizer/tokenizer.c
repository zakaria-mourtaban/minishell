#include "../../includes/minishell.h"

// #define TOKEN_CMD 0
// #define TOKEN_PIPE 1
// #define TOKEN_DLESS 2
// #define TOKEN_DGREAT 3
// #define TOKEN_LESS 4
// #define TOKEN_GREAT 5
// #define TOKEN_SPACE 6

t_tokens	*newnode(char *data, int type)
{
	t_tokens	*ptr;

	ptr = malloc(sizeof(t_tokens));
	if (ptr == NULL)
		return (NULL);
	ptr->content = ft_strdup(data);
	ptr->id = type;
	ptr->next = NULL;
	ptr->previous = NULL;
	return (ptr);
}

void	append(t_tokens **cmds, char *data, int type)
{
	t_tokens	*new_node;
	t_tokens	*tmp;

	new_node = NULL;
	new_node = newnode(data, type);
	printf("%s\n", data);
	if (*cmds == NULL)
	{
		*cmds = new_node;
		return ;
	}
	tmp = *cmds;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->previous = tmp;
}

void	printcmds(t_data *data)
{
	t_tokens	*tmp;
	const char	*type_names[] = {"DEFID", "WORD", "INFILE", "OUTFILE",
			"HEREDOC", "OUTAPPEND", "COMMAND", "OPERATOR", "PIPE", "SPACE"};

	tmp = data->cmdchain;
	while (tmp != NULL)
	{
		printf("[%s] %s\n", type_names[tmp->id], tmp->content);
		tmp = tmp->next;
	}
}

e_token	get_delimiter_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (TOKEN_IN_FILE);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_OUT_FILE);
	else if (ft_strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC_EOF);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_OUT_A_FILE);
	return (TOKEN_COMMAND);
}

void	tokenizer(char *input, t_data *data)
{
	int i = 0;
	char *buffer;
	int buf_i = 0;
	char quote = 0;
	int last_was_space = 0;

	data->cmdchain = NULL;
	buffer = ft_calloc(ft_strlen(input) + 1, sizeof(char));

	while (input[i] != '\0')
	{
		if (quote)
		{
			buffer[buf_i++] = input[i];
			if (input[i] == quote)
			{
				quote = 0;
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_COMMAND);
				buf_i = 0;
			}
			last_was_space = 0;
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (buf_i > 0)
			{
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_COMMAND);
				buf_i = 0;
			}
			quote = input[i];
			buffer[buf_i++] = input[i];
			last_was_space = 0;
		}
		else if (ft_strchr("|<> ", input[i]))
		{
			if (buf_i > 0)
			{
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_COMMAND);
				buf_i = 0;
			}

			if (input[i] == '<' && input[i + 1] == '<')
			{
				append(&data->cmdchain, "<<", TOKEN_HEREDOC_EOF);
				i++;
				last_was_space = 0;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				append(&data->cmdchain, ">>", TOKEN_OUT_A_FILE);
				i++;
				last_was_space = 0;
			}
			else if (input[i] == '>')
			{
				append(&data->cmdchain, ">", TOKEN_OUT_FILE);
				last_was_space = 0;
			}
			else if (input[i] == '<')
			{
				append(&data->cmdchain, "<", TOKEN_IN_FILE);
				last_was_space = 0;
			}
			else if (input[i] == '|')
			{
				append(&data->cmdchain, "|", TOKEN_PIPE);
				last_was_space = 0;
			}
		}
		else
		{
			if (last_was_space)
			{
				// Skip adding extra spaces
				buffer[0] = input[i];
				buffer[1] = '\0';
				append(&data->cmdchain, buffer, get_delimiter_type(buffer));
			}
			else
			{
				buffer[buf_i++] = input[i];
			}
			last_was_space = (input[i] == ' ');
		}
		i++;
	}

	// Handle any remaining data in the buffer
	if (buf_i > 0)
	{
		buffer[buf_i] = '\0';
		append(&data->cmdchain, buffer, TOKEN_COMMAND);
	}

	free(buffer);
}