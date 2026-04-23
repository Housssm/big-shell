#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0; // permet de sauvegarder le dernier message avant de quitter 

char	*get_line(void)
{
	char	*line_read;

	line_read = (char *) NULL;
	line_read = readline("shellinho:~$ ");
	if (line_read && *line_read)
		add_history(line_read);
	return (line_read);
}

void	handler(int sigtype)
{
	if (sigtype == SIGINT)
	{
		g_signal_received = sigtype;
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
	}
}

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

int	add_node(t_token **head, char *line, int beg, int end)
{
	t_token	*new;
	char	*value;

	value = malloc(sizeof(char) * (end - beg + 2));
	if (!value)
		return (0);
	ft_strlcpy(value, &line[beg], end - beg + 2);
	if (!is_blank(value))
		return (0);
	if (*head == NULL)
	{
		*head = malloc(sizeof(t_token));
		if (!(*head))
			return (ft_printf("Error Malloc node lexer", 2), 1);
		(*head)->value = ft_strdup(value);
		what_is_it((*head), value);
		(*head)->next = NULL;
	}
	else
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return (free(value), ft_printf("Error Malloc node lexer", 2), 1);
		new->value = ft_strdup(value);
		what_is_it(new, value);
		new->next = NULL;
		ft_add_last(*head)->next = new;
	}
	free(value);
	return (0);
}

int	new_token(char c, int *flag)
{
	int		i;
	char	array[] = {
		'|', '&', '<', '>', ' ', '	'};

	i = 0;
	if (c == 34 || c == 39)
	{
		if (*flag == 0)
			*flag = c;
		else if (*flag == c)
			*flag = 0;
		return (1);
	}
	while (array[i])
	{
		if (array[i] == c)
			return (1);
		i++;
	}
	return (0);
}


void	parse_line(t_token **cmd, char *line)
{
	int		i;
	int		flag;
	int		beg_pos;

	i = 0;
	flag = 0;
	beg_pos = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '	')
			i++;
		beg_pos = i;
		if (line[i] == 34 || line[i] == 39)
		{
			add_node(cmd, line , i, i);
			flag = line[i];
			i++;
			beg_pos = i;
			while (line[i] && line[i] != flag)
				i++;
			if (line[i] == flag)
			{
				add_node(cmd, line, beg_pos, i - 1);
				add_node(cmd, line, i, i);
				i++;
				beg_pos = i;
			}
			else
				return (ft_putstr_fd("Unclosed quote\n", 2));
		}
		while (line[i] && !new_token(line[i], &flag))
			i++;
		if (beg_pos < i)
			add_node(cmd, line, beg_pos, i);
		if (new_token(line[i], &flag))
		{
			add_node(cmd, line, i, i);
			i++;
			beg_pos = i;
		}
	}
}

// int	what_if_quote(t_token **cmd, char *line, int *flag, int beg_pos, int i)
// {

// 	add_node(cmd, line , i, i);
// 	*flag = line[i];
// 	i++;
// 	beg_pos = i;
// 	while (line[i] && line[i] != *flag)
// 		i++;
// 	if (line[i] == *flag)
// 	{
// 		add_node(cmd, line, beg_pos, i - 1);
// 		add_node(cmd, line, i, i);
// 		i++;
// 		beg_pos = i;
// 		return (0);
// 	}
// 	else
// 		return (ft_putstr_fd("Unclosed quote\n", 2), 1);
// }

// void	parse_line(t_token **cmd, char *line)
// {
// 	int		i;
// 	int		flag;
// 	int		beg_pos;

// 	i = 0;
// 	flag = 0;
// 	beg_pos = 0;
// 	while (line[i])
// 	{
// 		while (line[i] == ' ' || line[i] == '	')
// 			i++;
// 		beg_pos = i;
// 		if (line[i] == 34 || line[i] == 39)
// 		{
// 			if (what_if_quote(cmd, line, &flag, beg_pos, i))
// 				return ;
// 		}
// 		while (line[i] && !new_token(line[i], &flag))
// 			i++;
// 		if (beg_pos < i)
// 			add_node(cmd, line, beg_pos, i);
// 		if (new_token(line[i], &flag))
// 		{
// 			add_node(cmd, line, i, i);
// 			i++;
// 			beg_pos = i;
// 		}
// 	}
// }

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
	parse_line(cmd, line);
	boucle_str(cmd);
	clear_actual_command(cmd);
}

void	init(t_token *cmd)
{
	cmd->next = NULL;
	cmd->type = 0;
	cmd->value = NULL;
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
		boucle_str(&cmd);
		free(line);
		// free(cmd.value);
	}
	return (0);
}
