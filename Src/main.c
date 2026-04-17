#include "minishell.h"

volatile sig_atomic_t   signal_received = 0; // permet de sauvegarder le dernier message avant de quitter 

/* 
 Fonction qui creer un noeud qui correspond a un s_token et retourb int = taille du token 
	-identifier  les tokens fixes
	-enum WORD pour tout les autres avant de les traiter sur le parsing
	-si une quote jouvre jusqua la prochaine quote 
	-la  prochaine ligne de commande sarrete quand il y a  un isspace ou un charactere special : |, $ , ', ";
 */


//Fonction pour extraire la ligne de commande
char	*get_line() 
{
	char *line_read;

	line_read = (char *)NULL;
	line_read = readline("shellinho:~$ ");
	if (line_read && *line_read)	//verifie que la ligne existe et nest pas vide pour la stocker dans lhistorique
		add_history(line_read);
	return (line_read);
}

void	handler(int sigtype) // function for the main, need one for the parent and the child
{
	if (sigtype == SIGINT) // si  on a CTRL + C on dit que lon change de ligne et on la redisplay
	{
		signal_received = sigtype;
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
	}
}

void	define_cmd(t_token *cmd, t_type opcode, char *str)
{
		cmd->type = opcode;
		cmd->value = str;
		cmd->next = NULL;
}

void	what_is_it(t_token *cmd, char *str)
{
	if	(str[0] == '|')
		define_cmd(cmd, PIPE, str);
	else if	(str[0] == '<' && !str[1])
		define_cmd(cmd, INREDIR, str);
	else if	(str[0] == '>' && !str[1])
		define_cmd(cmd, OUTREDIR, str);
	else if	(str[0] == '<' && str[1] == '<')
		define_cmd(cmd, HEREDOC, str);
	else if	(str[0] == '>' && str[1] == '>')
		define_cmd(cmd, APPOUTREDIR, str);
	else if ((str[0] >= 'a' && str[0] <= 'z') || str[0] == '-')
		define_cmd(cmd, WORD, str);
	// else if (str[0] == 34 | str[0] == 39);
	// 	define_cmd
	else
		return /* (printf("Probleme with the characterer\n")) */; // gerer ce cas d'erreur 
}


int	add_node(t_token **head, char *line, int beg, int end)
{
	t_token	*new;
	char	*value;

	value = malloc(sizeof(char) * (end - beg) + 1);
	if (!value)
		return (0);
	ft_strlcpy(value, &line[beg], end - beg);
	if(*head == NULL)
	{
		*head = malloc(sizeof(t_token));
		if (!(*head))
			return ( ft_printf("Error Malloc node lexer", 2), 1);
		(*head)->value = ft_strdup(value);
		what_is_it((*head), value);
		(*head)->next = NULL;
		printf(" un %s\n", value);
	}
	else
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return ( ft_printf("Error Malloc node lexer", 2), 1);
		new->value = ft_strdup(value);
		printf(" deux %s\n", value);
		what_is_it(new, value);
		new->next = NULL;
		(*head)->next = new;

	}

	return (0);
}

void	parse_line(t_token *cmd, char *line)
{
	int	i;
	int	flag;
	int	beg_pos;

	i = 0;
	flag = 0;
	beg_pos = 0;
	while (line[i])
	{
		if (line[i] == 34 || line[i] == 39)
		{
			flag = line[i];
			beg_pos = i +1;
			while(line[i])
			{
				if (line[i] == '\0')
					break; //return error unclosed quote
				if (line[i] == flag)
					add_node(&cmd, line, beg_pos, i);
				i++;
			}
		}
		i++;
	}
}



// size_t	parse_line(t_token *cmd, char *line)
// {
// 	size_t	len;
// 	size_t	i;
// 	size_t	j;
// 	char	*value;
// 	t_token	*cursor;

// 	if (!cmd || !line)
// 		return (1);
// 	i = 0;
// 	len = 0;
// 	cursor = cmd;
// 	while (cursor->next)
// 		cursor = cursor->next;
// 	while (line[i])
// 	{
// 		while (line[i] && ((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
// 			i++;
// 		if (!line[i])
// 			break ;
// 		j = i;
// 		while (line[i] && !((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
// 			i++;
// 		value = malloc((i - j) + 1);
// 		if (!value)
// 			return (1);
// 		ft_memcpy(value, &line[j], i - j);
// 		value[i - j] = '\0';
// 		len = add_node(&cursor, value);
// 		free(value);
// 		if (len != 0)
// 			return (len);
// 		if (cursor->next)
// 			cursor = cursor->next;
// 	}
// 	return (0);
// }



void	lexer(t_token *cmd, char *line)
{
	parse_line(cmd, line);

}
void	init(t_token *cmd)
{
	cmd->next= NULL;
	cmd->type = 0;
	cmd->value = NULL;
}

void	boucle_str(t_token **head)
{
	t_token *current;

	current = *head;
	while(head)
	{
		printf("[%d, %s]\n",current->type, current->value);
		current = (*head)->next;
	}
}



int	main(void)
{
	char					*line;
	struct sigaction		action;
	t_token					*cmd;
	// t_parse					parse;

	ft_memset(&action, 0, sizeof(action));
	ft_memset(&cmd, 0, sizeof(cmd));
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
		lexer(cmd, line);
		// boucle_str(&cmd);
	
		free(line);
		// free(cmd.value);
	}
	return (0);
}


