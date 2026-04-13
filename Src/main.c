#include "minishell.h"



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

void	handler(int sigtype)
{
	if (sigtype == SIGQUIT) // si on a CTRL + \ on ne fait rien
	{
		return ;
	}
	if (sigtype == SIGINT) // si  on a CTRL + C on dit que lon change de ligne et on la redisplay
	{
		signal_received = sigtype;
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
		// printf("^");		// printf("^");
		// printf("C");

		// printf("C");
	}
}

int	main(void)
{
	char				*line;
	struct sigaction	action;
	volatile sig_atomic_t   signal_received;

	action.sa_handler = handler;
	action.sa_flags = 0;
	signal_received	= 0;
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
		sigaction(SIGQUIT, &action, NULL);



		printf("shellinho:~$~%s\n", line);
		free(line);
	}
	return (0);
}