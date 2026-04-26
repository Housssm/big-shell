/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:47 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:35:47 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*************************** TEMPORAIRE ************************/

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

// void	what_is_ptype(t_tree *tree, t_token *cmd)
// {
// 	if (cmd->type == PIPE)
// 		tree->parse_type = PIPE_PARS;
// 	else if (cmd->type == INREDIR)




// 	else if (str[0] == '<' && !str[1])
// 		define_cmd(cmd, INREDIR);
// 	else if (str[0] == '>' && !str[1])
// 		define_cmd(cmd, OUTREDIR);
// 	else if (str[0] == '<' && str[1] == '<')
// 		define_cmd(cmd, HEREDOC);
// 	else if (str[0] == '>' && str[1] == '>')
// 		define_cmd(cmd, APPOUTREDIR);
// 	else if (str[0] == 34)
// 		define_cmd(cmd, WEAK_QUOTE);
// 	else if (str[0] == 39)
// 		define_cmd(cmd, STRONG_QUOTE);
// 	else
// 		define_cmd(cmd, WORD);

// }
// t_tree	*init_tree(t_tree **tree, t_token **cmd, int count)
// {
// 	t_tree	*new_branch;


// 	new_branch = malloc(sizeof(t_tree));
// 	if (!new_branch)
// 		return (NULL);
// 	// what_is_ptype(tree, cmd);
// 	new_branch->ac = count;

// 	return (0);
// }

void	boucle_cmd(t_tree **tree, t_token **cmd)
{
	t_token	*current;
	size_t	count;
	
	(void)tree;
	count = 0;
	current = *cmd;
	while (current)
	{
		if (current->type == PIPE)
		{
			printf("ls\n");
			// init_tree(tree, cmd, count);
		}
		current = current->next;
		count += 1;
	}
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

/*************************** TEMPORAIRE ************************/

/*Fonction pour le parsing cas des pipes*/

/* fonction qui se balade dans les liste chainee et cherche un pipe tout en comptant;
fonction qui prends en parametre la tete et recommence det ecrit dans le tree en allant jusquau int
on dit que la nouvelle tete est la commande qui se trouve juste apres le pipe et on rappele la fonction pour recherche le pipe
 */

 void	parser(t_tree **tree, t_token **cmd)
{
	boucle_cmd(tree, cmd);

}


void	lexer(t_tree **tree, char *line)
{
	t_token				*cmd;

	cmd = NULL;
	if (parse_line(&cmd, line))
		return (clear_actual_command(&cmd));
	boucle_str(&cmd);
	parser(tree, &cmd);
	clear_actual_command(&cmd);
}

int	main(void)
{
	char				*line;
	struct sigaction	action;
	// t_token				*cmd;
	t_tree				*tree;

	ft_memset(&action, 0, sizeof(action));
	// cmd = NULL;
	tree = NULL;
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
		lexer(&tree, line);
		free(line);
	}
	return (0);
}
