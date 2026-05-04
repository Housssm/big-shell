// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/04/25 13:35:47 by hoel-har          #+#    #+#             */
// /*   Updated: 2026/04/25 13:35:47 by hoel-har         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

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
void print_tree_line(t_tree *tree)
{
	if (!tree)
	{
		printf("tree = NULL\n");
		return;
	}
	printf("--------------BRANCH-----------------------\n");

	// Ligne 1: Type
	printf("	type = ");
	switch (tree->parse_type)
	{
		case PIPE_PARS:			printf("PIPE_PARS"); break;
		case INREDIR_PARS:		printf("INREDIR_PARS"); break;
		case OUTREDIR_PARS:		printf("OUTREDIR_PARS"); break;
		case HEREDOC_PARS:		printf("HEREDOC_PARS"); break;
		case APPOUTREDIR_PARS:	printf("APPOUTREDIR_PARS"); break;
		case WORD_PARS:			printf("WORD_PARS"); break;
		case WORD_QUOTE_PARS:	printf("WORD_QUOTE_PARS"); break;
		default:				printf("UNKNOWN");
	}
	printf("\n");
	
	// Ligne 2: ac
	printf("	ac = %d\n", tree->ac);
	
	// Ligne 3: av
	printf("	av = [");
	if (tree->av && tree->ac > 0)
	{
		for (int i = 0; i < tree->ac; i++)
		{
			if (tree->av[i])
				printf("[%s]", tree->av[i]);
			else
				printf("[NULL]");
			if (i < tree->ac - 1)
				printf(" ");
		}
	}
	else
		printf("NULL");
	printf("]\n");
	
	// Ligne 4: fds
	printf("	fds = %s\n", (tree->fds != NULL) ? "oui" : "NULL");
	
	// Ligne 5: right
	printf("	right = %s\n", (tree->right != NULL) ? "non NULL" : "NULL");
	
	// Ligne 6: left
	printf("	left = %s\n", (tree->left != NULL) ? "non NULL" : "NULL");

	printf("-------------------------------------------\n");

}
/*************************** TEMPORAIRE ************************/


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

void free_tree(t_tree *tree)
{
	if (!tree)
		return;
	free_tree(tree->left);
	free_tree(tree->right);    
	if (tree->av)
		free_split(tree->av);
	if (tree)
		free(tree);
}

void	print_tree(t_tree *tree)
{
	printf("Head : \n");
	print_tree_line(tree);
	if (tree->left != NULL)
	{
		printf("Left : \n");
		print_tree_line(tree->left);
	}
	if (tree->right != NULL)
	{
		printf("Right : \n");
		print_tree_line(tree->right);
	}
}

void	what_is_ptype(t_tree *tree, t_token *cmd)
{
	if (cmd->type == PIPE)
		tree->parse_type = PIPE_PARS;
	else if (cmd->type == INREDIR)
		tree->parse_type = INREDIR_PARS;
	else if (cmd->type == OUTREDIR)
		tree->parse_type = OUTREDIR_PARS;
	else if (cmd->type == HEREDOC)
		tree->parse_type = HEREDOC_PARS;
	else if (cmd->type == APPOUTREDIR)
		tree->parse_type = APPOUTREDIR_PARS;
	else if (cmd->type == WORD && (cmd->value[0] == 34 || cmd->value[0] == 39))
		tree->parse_type = WORD_QUOTE_PARS;
	else
		tree->parse_type = WORD_PARS;
}

int	value_from_list_to_tree(t_tree *branch, t_token **cmd, size_t count)
{
	size_t	i;
	t_token	*current;

	i = 0;
	current = *cmd;
	while (i < count)
	{
		branch->av[i] = ft_strdup(current->value);
		if (!branch->av[i])
			return (free_split(branch->av) , 1);
		current = current->next;
		i++;
	}
	branch->av[i] = NULL;
	return (0);
}

t_tree	*left_branch(t_tree *tree, t_token **cmd, size_t count) // commence depuis la **head de tree jusquqau count et la prochaine commande apres le pipe devient la nouvelle head
{
	t_tree	*left;
	left = malloc(sizeof(t_tree));
	if (!left)
		return (free_tree(tree), NULL);
	left->av = malloc(sizeof(char *) * (count + 1));
	if (!left->av)
		return (free(left), free_tree(tree), NULL);
	left->ac = count;
	left->fds = 0;
	left->left = NULL;
	left->right = NULL;
	what_is_ptype(left, ((*cmd)));
	if (value_from_list_to_tree(left, cmd, count))
		return(free_split(left->av) , free(left), free_tree(tree), NULL);
	return (left);
}


// void	actualise_cmd(t_token **cmd, size_t count)
// {
// 	t_token	*current;
// 	size_t	i;

// 	i = 0;
// 	while (i < count + 1)
// 	{
// 		current = (*cmd)->next;
// 		free((*cmd)->value);
// 		free((*cmd));
// 		*cmd = current;
// 		i++;
// 	}
// 	*cmd = current;
// }
// t_tree	*create_branch_pipe(t_tree **tree, t_token **cmd, size_t count) // creer la branche avec le pipe en son centre, premiere = head sinon elle est a droite du precedant head
// {
// 	t_tree	*new_branch;

// 	new_branch = tree
// 	new_branch = malloc(sizeof(t_tree));
// 	if (!new_branch)
// 		return (NULL);
// 	new_branch->ac = 1;
// 	new_branch->av = malloc(sizeof(char *) * (count + 1));
// 	if (!new_branch->av)
// 		return (free_tree(new_branch), NULL);
// 	new_branch->av[0] = ft_strdup("|");
// 	if (!new_branch->av[0])
// 		return (free_tree(new_branch), NULL);
// 	new_branch->av[1] = NULL;
// 	new_branch->parse_type = PIPE_PARS;
// 	new_branch->fds = 0;
// 	new_branch->left = left_branch(cmd, count);
// 	if (!new_branch->left)
// 		return (free_tree(new_branch), NULL);
// 	// actualise_cmd(cmd, count);
// 	// printf("second\n\n");
// 	// boucle_str(cmd);
// 	new_branch->right = NULL;
// 	// new_branch->right = find_pipe(tree, cmd);
// 	// print_tree_line(new_branch);
// 	// print_tree(new_branch);
// 	free_tree(new_branch);
// 	return (0);
// }

// int branch_no_pipe(t_tree **tree, t_token **cmd,  size_t count)
// {
// 	t_tree *no_pipe;

// 	no_pipe = malloc(sizeof(t_tree));
// 	if (!no_pipe)
// 		return (1);
// 	no_pipe->parse_type = NO_PIPE;
// 	no_pipe->ac = 1;
// 	no_pipe->av = malloc(sizeof(char *) * 2);
// 	if (!no_pipe->av)
// 		return (free(no_pipe), 1);
// 	no_pipe->av[0] = ft_strdup("\\");
// 	if (!no_pipe->av[0])
// 		return (free(no_pipe), free_split(no_pipe->av), 1);
// 	no_pipe->av[1] = NULL;
// 	no_pipe->fds = 0;
// 	no_pipe->right = NULL;
// 	no_pipe->left = left_branch(cmd, count);
// 	return (0);
// }

// int	find_pipe(t_tree **tree, t_token **cmd) // trouve le prochain pipe
// {
// 	t_token	*current;
// 	size_t	count;
	
// 	// (void)tree;
// 	count = 0;
// 	current = *cmd;
// 	while (current->next)
// 	{
// 		if (current->type == PIPE)
// 		{
// 			if (create_branch_pipe(tree, cmd, count))
// 				return (1);
// 				//if there is no pipe alors head = NO PIPE et a gauxhe il y a toute la commande 
// 		}
// 		current = current->next;
// 		count += 1;
// 		if (current->next == NULL || current->type != PIPE)
// 		{
// 			if (branch_no_pipe(tree, cmd, count))
// 				return (1);
// 		}
// 	}
// 	return (0);
// }

//  int	parser(t_tree *tree, t_token **cmd)
// {
// 	if (find_pipe(tree, cmd))
// 		return (1);
// 	return (0);
// }


bool	search_pipe(t_token **cmd, size_t *count)
{
	t_token	*current;
	
	*count = 0;
	current = *cmd;
	while (current->next)
	{
		if (current->type == PIPE)
			return (true);
		current = current->next;
		*count += 1;
	}
	if (current->type == PIPE)
		return (ft_putstr_fd("Invalid command\n",1), false);
	return (false);
}


t_tree	*new_pipe(t_tree *tree, t_token **cmd, size_t *count)
{
	t_tree	*branch_pipe;

	if (search_pipe(cmd, count) == false)
		return (NULL);
	branch_pipe = malloc(sizeof(t_tree));
	if (!branch_pipe)
		return (free_tree(tree), free_tree(tree), NULL);
	branch_pipe->parse_type = PIPE_PARS;
	branch_pipe->ac = 0;
	branch_pipe->av = malloc(sizeof(char *) * 2);
	if (!branch_pipe->av)
		return (free(branch_pipe), free_tree(tree), NULL);
	branch_pipe->av[0] = ft_strdup("|");
	if (!branch_pipe->av[0])
		return (free_split(branch_pipe->av), free(branch_pipe), free_tree(tree), NULL);
	branch_pipe->av[1] = NULL;
	branch_pipe->fds = 0;
	branch_pipe->left = NULL;
	branch_pipe->right = NULL;
	return (branch_pipe);
}

t_tree	*no_pipe_tree(t_tree *tree , t_token **cmd, size_t *count)
{
	t_tree	*branch_pipe;

	branch_pipe = malloc(sizeof(t_tree));
	if (!branch_pipe)
		return (ft_putstr_fd("Error malloc\n", 2), NULL);
	branch_pipe->parse_type = NO_PIPE_PARS;
	branch_pipe->ac = *count;
	branch_pipe->av = malloc(sizeof(char *) * (*count + 1));
	if (!branch_pipe->av)
		return (free(branch_pipe), free_tree(tree), NULL);
	if (value_from_list_to_tree(branch_pipe, cmd, *count))
		return (free_split(branch_pipe->av), free(branch_pipe), NULL);
	branch_pipe->fds = 0; //fonction pour calculer les fds
	branch_pipe->left = NULL;
	branch_pipe->right = NULL;
	return (branch_pipe);
}

int	parser(t_tree **tree, t_token **cmd)
{
	size_t	count;
	// t_token	*new_head;

	count = 0;
	if (!*tree)
		*tree = new_pipe(*tree, cmd, &count);
	if (!*tree)
	{
		*tree = no_pipe_tree(*tree, cmd, &count);
		if (!*tree)
			return (free_tree(*tree), 1);
		return(0);
	}
	if (!(*tree)->left)
		(*tree)->left= left_branch(*tree, cmd, count);
	// if (!tree->left)
	// 	return (free_tree(tree), 1);
	// new_head = (*cmd)->next;
	// if (!tree->right)
	// parser(tree->right, &new_head);
	print_tree(*tree);
	return (0);
}

/* 
	creer un node avec le pipe si il ny a rien return null
	si pas de pipe alors on creer un noeud simple --> cas pas de pipe 
	si pas de left alrs on le creer
		si il y a une erreyr alors on return
	nouvelle head = ce quil y a apres le pipe
	si droite existe pas et quil y a une pipe 
		on dit que tree de droite =  new pipe 
		on rappelle la fonction sur tree->right
		return
	dans le cas ou il ny a pas de pipe a droite alors on rempli le noeud de droite simple 
*/


int	lexer(t_tree **tree, char *line)
{
	t_token	*cmd;
	int		return_pars_line;

	cmd = NULL;
	return_pars_line = parse_line(&cmd, line);
	if (return_pars_line == 2)
		return (clear_actual_command(&cmd), free_tree(*tree), 0);
	if (return_pars_line != 0)
		return (clear_actual_command(&cmd), 1);
	// boucle_str(&cmd);
	if (parser(tree, &cmd))
		return (clear_actual_command(&cmd), 2);
	clear_actual_command(&cmd);
	free_tree(*tree);
	*tree = NULL;
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char				*line;
	struct sigaction	action;
	t_tree				*tree;

	(void)ac;
	(void)av;
	(void)env;
	ft_memset(&action, 0, sizeof(action));
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
		if (lexer(&tree, line))
		{	
			free(line);
			free_tree(tree);
			return (1);
		}
		free(line);
	}
	return (0);
}
