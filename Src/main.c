#include "minishell.h"



//Fonction pour extraire la ligne de commande
char	*get_line() 
{
	char *line_read;

	line_read = (char *)NULL;
	line_read = readline("shellinho$ ");
	if (line_read && *line_read)	//verifie que la ligne existe et nest pas vide pour la stocker dans lhistorique
		add_history(line_read);
	return (line_read);
}


int	main(void)
{
	char	*line;

	while (1)
	{
		line = get_line();
		if (!line)
		{
			printf("exit");
			rl_clear_history();
			exit (0);
		}
		printf("%s\n", line);
		free(line);
	}
	return (0);
}