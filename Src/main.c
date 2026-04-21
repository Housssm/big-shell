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

void	boucle_str(t_token **head) // Fonction pour iterer le long de mes commandes pour voir si la copie a ete bien fait. La fonction a un but de debeug uniquement
{
	t_token *current;

	current = *head;
	while(current)
	{
		printf("[%d, %s]\n",current->type, current->value);
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

void	define_cmd(t_token *cmd, t_type opcode) // fonctionne avec la fonction juste en dessous afib de remplir la structure
{
		cmd->type = opcode;
		cmd->next = NULL;
}

void	what_is_it(t_token *cmd, char *str) // Permet 
{
	if	(str[0] == '|')
		define_cmd(cmd, PIPE);
	else if	(str[0] == '<' && !str[1])
		define_cmd(cmd, INREDIR);
	else if	(str[0] == '>' && !str[1])
		define_cmd(cmd, OUTREDIR);
	else if	(str[0] == '<' && str[1] == '<')
		define_cmd(cmd, HEREDOC);
	else if	(str[0] == '>' && str[1] == '>')
		define_cmd(cmd, APPOUTREDIR);
	else
		define_cmd(cmd, WORD);
	// else if ((str[0] >= 'a' && str[0] <= 'z') || str[0] == '-')
	// 	define_cmd(cmd, WORD);
	// else if (str[0] == 34 | str[0] == 39);
	// 	define_cmd
}



int	add_node(t_token **head, char *line, int beg, int end) // creer un noeud et lajoute a la suite du precedant, chaque noeud correspond a un "mot/quote"
{
	t_token	*new;
	char	*value;

	value = malloc(sizeof(char) * (end - beg + 2));
	if (!value)
		return (0);
	ft_strlcpy(value, &line[beg], end - beg + 2);
	if(*head == NULL)
	{
		*head = malloc(sizeof(t_token));
		if (!(*head))
			return ( ft_printf("Error Malloc node lexer", 2), 1);
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

void	parse_line(t_token **cmd, char *line) // fonction permettant ditterer au  sein de la ligne de commande afin de pouvoir separer les differentes commandes
{
	int	i;
	int	flag;
	int	beg_pos;

	i = 0;
	flag = 0;
	while (line[i])
	{
		if (line[i] == 39 || line[i] == 34 )
		{
			flag = line[i];
			beg_pos = i + 1;
			while (line[i] && line[i] != flag)
				i++;
			if (line[i] == '\0')
				return (ft_putstr_fd("Unclosed quote", 2));
			if (beg_pos < i)
			{	
				add_node(cmd, line, beg_pos, i - 1);
				flag = 0;
			}
		}
		while (line[i] && (line[i] == 32 || line[i] == 9))
			i++;
		beg_pos = i;
		while (line[i] && !(line[i] == 32 || line[i] == 9))
			i++;
		if (beg_pos < i)
			add_node(cmd, line,beg_pos, i - 1);

	}
}



void	clear_actual_command(t_token **head) // permet de supprimer les listes chaines correspondant a une ligne afin que chaque ligne soit independante
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
	cmd->next= NULL;
	cmd->type = 0;
	cmd->value = NULL;
}





int	main(void)
{
	char					*line;
	struct sigaction		action;
	t_token					*cmd;
	// t_parse					parse;

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
		// boucle_str(&cmd);
	
		free(line);
		// free(cmd.value);
	}
	return (0);
}


