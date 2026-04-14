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


	// tab_result = split(line);
	// return (tab_result);
}


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
