#include "minishell.h"

volatile sig_atomic_t   signal_received = 0; // permet de sauvegarder le dernier message avant de quitter 


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

void	what_is_it(char *str, t_token *cmd)
{
	if	(str[0] == '|')
		define_cmd(cmd, PIPE, str);
	else if	(str[0] == '<' && !str[1])
		define_cmd(cmd, INREDIR, str);
	else if	(str[0] == '>' && !str[1])
		define_cmd(cmd, HEREDOC, str);
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


void	split_line(char *line, t_token *cmd)
{
	char	**tab;
	int		i;

	i = 0;
	tab = ft_split(line, ' ');
	while(tab[i])
	{
		what_is_it(tab[i], cmd);
		i++;
		printf("[%d, %s]\n",cmd->type, cmd->value);
	}
}

int	is_quote(char letter, int flag)
{
	if (letter == 34 || flag == 0 || flag == 34)
	{
		if (flag == 0)
			flag = 34;
		else
			flag = 0;
		return (flag);
	}
	if (letter == 39 || flag == 0 || flag == 0)
	{
		if (flag == 0)
			flag = 39;
		else
			flag = 0;
		return (flag);
	}
	return (0);
}

char	*ft_strcncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	**split_line(char *line, t_token *cmd)
{
	char	**result_tab;
	int		i;
	int		j;
	int		quote;

	quote = 0;
	j = 0;
	i = 0;
	while (line[i])
	{
		//ouvre guilemt fleag va jusququ prochain ferme le flkag et passe au prochain string 
		if (is_quote(line[i], &quote))
		{
			while (is_quote(line[i], &quote) != quote)
				i++;
			if (line[i] == '\0')
				break ;
			if (os )
			i++;
		}
	return (result_tab);
	}
}

	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] == '\0')
			break ;
		while (str[i] && str[i] != c)
			i++;
		count++;





int	main(void)
{
	char					*line;
	struct sigaction		action;
	t_token					cmd;

	ft_memset(&action, 0, sizeof(action));
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
		split_line(line, &cmd);
		
		free(line);
		// free(cmd.value);
	}
	return (0);
}
