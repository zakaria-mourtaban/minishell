#include "../includes/minishell.h"

#define TOKEN_CMD 0
#define TOKEN_PIPE 1
#define TOKEN_DLESS 2
#define TOKEN_DGREAT 3
#define TOKEN_LESS 4
#define TOKEN_GREAT 5
#define TOKEN_SPACE 6

t_cmds	*newnode(char *data, int type)
{
	t_cmds	*ptr;

	ptr = malloc(sizeof(t_cmds));
	if (ptr == NULL)
		return (NULL);
	ptr->value = ft_strdup(data);
	ptr->type = type;
	ptr->next = NULL;
	return (ptr);
}

void	append(t_cmds **cmds, char *data, int type)
{
	t_cmds	*new_node;
	t_cmds	*tmp;

	new_node = newnode(data, type);
	if (*cmds == NULL)
	{
		*cmds = new_node;
		return ;
	}
	tmp = *cmds;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	printcmds(t_data *data)
{
	t_cmds		*tmp;
	const char	*type_names[] = {"CMD", "PIPE", "DLESS", "DGREAT", "LESS",
			"GREAT", "SPACE"};

	tmp = data->cmdchain;
	while (tmp != NULL)
	{
		printf("[%s] %s\n", type_names[tmp->type], tmp->value);
		tmp = tmp->next;
	}
}

int	get_delimiter_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_DLESS);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_DGREAT);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_LESS);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_GREAT);
	if (ft_strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	return (TOKEN_CMD);
}

void	tokenizer(char *input, t_data *data)
{
	int i = 0;
	char *buffer;
	int buf_i = 0;
	char quote = 0;

	data->cmdchain = NULL;
	buffer = ft_calloc(ft_strlen(input), sizeof(char));
	while (input[i] != '\0')
	{
		if (quote)
		{
			buffer[buf_i++] = input[i];
			if (input[i] == quote)
			{
				quote = 0;
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_CMD);
				buf_i = 0;
			}
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (buf_i > 0)
			{
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_CMD);
				buf_i = 0;
			}
			quote = input[i];
			buffer[buf_i++] = input[i];
		}
		else if (ft_strchr("|<> ", input[i]))
		{
			if (buf_i > 0)
			{
				buffer[buf_i] = '\0';
				append(&data->cmdchain, buffer, TOKEN_CMD);
				buf_i = 0;
			}
			if (input[i] == '<' && input[i + 1] == '<')
			{
				append(&data->cmdchain, "<<", TOKEN_DLESS);
				i++;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				append(&data->cmdchain, ">>", TOKEN_DGREAT);
				i++;
			}
			else
			{
				buffer[0] = input[i];
				buffer[1] = '\0';
				append(&data->cmdchain, buffer, get_delimiter_type(buffer));
			}
		}
		else
		{
			buffer[buf_i++] = input[i];
		}
		i++;
	}

	if (buf_i > 0)
	{
		buffer[buf_i] = '\0';
		append(&data->cmdchain, buffer, TOKEN_CMD);
	}
}