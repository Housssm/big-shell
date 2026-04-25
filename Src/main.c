#include "minishell.h"

// volatile sig_atomic_t	g_signal_received = 0; // save lst msg  

// char	*get_line(void)
// {
// 	char	*line_read;

// 	line_read = (char *) NULL;
// 	line_read = readline("shellinho:~$ ");
// 	if (line_read && *line_read)
// 		add_history(line_read);
// 	return (line_read);
// }

// void	handler(int sigtype)
// {
// 	if (sigtype == SIGINT)
// 	{
// 		g_signal_received = sigtype;
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		printf("\n");
// 		rl_redisplay();
// 	}
// }

void	boucle_str(t_token **head)
{
	t_token	*current;

	current = *head;
	while (current)
	{
		printf ("[%d, %s]\n", current->type, current->value);
		current = current->next;
	}
}

t_token	*ft_add_last(t_token *head)
{
	t_token	*temp;

	if (!head)
		return (NULL);
	temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	define_cmd(t_token *cmd, t_type opcode)
{
	cmd->type = opcode;
	cmd->next = NULL;
}

void	what_is_it(t_token *cmd, char *str) // Permet 
{
	if (str[0] == '|')
		define_cmd(cmd, PIPE);
	else if (str[0] == '<' && !str[1])
		define_cmd(cmd, INREDIR);
	else if (str[0] == '>' && !str[1])
		define_cmd(cmd, OUTREDIR);
	else if (str[0] == '<' && str[1] == '<')
		define_cmd(cmd, HEREDOC);
	else if (str[0] == '>' && str[1] == '>')
		define_cmd(cmd, APPOUTREDIR);
	else if (str[0] == 34)
		define_cmd(cmd, WEAK_QUOTE);
	else if (str[0] == 39)
		define_cmd(cmd, STRONG_QUOTE);
	else
		define_cmd(cmd, WORD);
}

int	is_blank(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '	')
			return (1);
		i++;
	}
	return (0);
}

t_token	*create_token_from_line(char *line, int beg, int end)
{
	char	*value;
	t_token	*token;

	value = malloc(sizeof(char) * (end - beg + 2));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &line[beg], end - beg + 2);
	if (!is_blank(value))
		return (free(value), NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(value), ft_printf("Error Malloc node lexer", 2), NULL);
	token->value = ft_strdup(value);
	what_is_it(token, token->value);
	token->next = NULL;
	free(value);
	return (token);
}

int	add_node(t_token **head, char *line, int beg, int end)
{
	t_token	*new_token;

	new_token = create_token_from_line(line, beg, end);
	if (!new_token)
		return (1);
	if (!*head)
		*head = new_token;
	else
		ft_add_last(*head)->next = new_token;
	return (0);
}

int	new_token(char c, int *flag)
{
	size_t		i;
	char		*array;

	array = "|&<> \t";
	if (*flag != 0)
		return (0);
	i = 0;
	while (array[i])
	{
		if (array[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	handle_quote(t_token **cmd, char *line, int *i, int *in_quote)
{
	char	quote_char;
	int		start;

	quote_char = line[*i];
	add_node(cmd, line, *i, *i);
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (!line[*i])
		return (ft_putstr_fd("Unclosed quote\n", 2), 1);
	add_node(cmd, line, start, *i - 1);
	add_node(cmd, line, *i, *i);
	(*i)++;
	*in_quote = 0;
	return (0);
}

// Ajoute un token normal (mot) si présent
void	add_word_token(t_token **cmd, char *line, int start, int end)
{
	if (start < end)
		add_node(cmd, line, start, end - 1);
}

// Traite un token (mot ou début de quote)
int	process_token(t_token **cmd, char *line, int *i, int *in_quote)
{
	int	start;

	start = *i;
	if (line[*i] == 34 || line[*i] == 39)
	{
		*in_quote = 1;
		return (handle_quote(cmd, line, i, in_quote));
	}
	while (line[*i] && !new_token(line[*i], in_quote))
		(*i)++;
	add_word_token(cmd, line, start, *i);
	return (0);
}

// Tokenise toute la ligne
int	parse_line(t_token **cmd, char *line)
{
	size_t	i;
	int		in_quote;

	i = 0;
	in_quote = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (process_token(cmd, line, (int *)&i, &in_quote))
			return (1);
		if (line[i] && !in_quote && new_token(line[i], &in_quote))
		{
			add_node(cmd, line, i, i);
			i++;
		}
	}
	return (0);
}

void	clear_actual_command(t_token **head)
{
	t_token	*current;

	while ((*head))
	{
		current = (*head)->next;
		free((*head)->value);
		free((*head));
		*head = current;
	}
	*head = NULL;
}

void	lexer(t_token **cmd, char *line)
{
	if (parse_line(cmd, line))
		return (clear_actual_command(cmd));
	boucle_str(cmd);
	clear_actual_command(cmd);
}


int	main(void)
{
	char					*line;
	struct sigaction		action;
	t_token					*cmd;

	ft_memset(&action, 0, sizeof(action));
	cmd = NULL;
	action.sa_handler = handler;
	while (1)
	{
		line = get_line();
		if (!line)
		{
			printf("exit");
			rl_clear_history();
			exit (0);
		}
		sigaction(SIGINT, &action, NULL);
		signal(SIGQUIT, SIG_IGN);
		lexer(&cmd, line);
		free(line);// +fonction qui free tout 
	}
	return (0);
}
