/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:48:12 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/06 23:48:52 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	boucle_str(t_token **head)
// {
// 	t_token	*current;

// 	current = *head;
// 	while (current)
// 	{
// 		printf ("[%d, %s]\n", current->type, current->value);
// 		current = current->next;
// 	}
// }

// static void	print_node_type(t_pars_type type)
// {
//     switch (type)
//     {
//         case PIPE_PARS:			printf("PIPE_PARS\n"); break;
//         case NO_PIPE_PARS:		printf("NO_PIPE_PARS\n"); break;
//         case INREDIR_PARS:		printf("INREDIR_PARS\n"); break;
//         case OUTREDIR_PARS:		printf("OUTREDIR_PARS\n"); break;
//         case HEREDOC_PARS:		printf("HEREDOC_PARS\n"); break;
//         case APPOUTREDIR_PARS:	printf("APPOUTREDIR_PARS\n"); break;
//         case WORD_PARS:			printf("WORD_PARS\n"); break;
//         case WORD_QUOTE_PARS:	printf("WORD_QUOTE_PARS\n"); break;
//         default:				printf("UNKNOWN\n");
//     }
// }

// static void	print_tree_argv(t_tree *tree)
// {
//     if (!tree->av || tree->ac == 0)
//     {
//         printf("NULL\n");
//         return;
//     }
//     printf("[");
//     for (int i = 0; i < tree->ac; i++)
//     {
//         if (tree->av[i])
//             printf("[%s]", tree->av[i]);
//     }
//     printf("]\n");
// }

// static void	print_tree_recursive(t_tree *tree, char *prefix)
// {
//     char left_pfx[1024];
//     char right_pfx[1024];
//     char child_pfx[1024];

//     ft_strlcpy(child_pfx, prefix, sizeof(child_pfx));
//     ft_strlcat(child_pfx, "│    ", sizeof(child_pfx));

//     // Type
//     printf("%s└─── type: ", prefix);
//     print_node_type(tree->parse_type);

//     // Argc
//     printf("%s     ├─── argc: %d\n", prefix, tree->ac);

//     // Argv
//     printf("%s     ├─── argv: ", prefix);
//     print_tree_argv(tree);

//     // fds
//     printf("%s     ├─── fds\n%s     │    └─── %s\n", prefix, prefix, tree->fds ? "YES" : "NULL");

//     // subshell
//     printf("%s     ├─── subshell\n%s     │    └─── NULL\n", prefix, prefix); // Fixe à NULL pour l'instant

//     // left
//     printf("%s     ├─── left\n", prefix);
//     ft_strlcpy(left_pfx, prefix, sizeof(left_pfx));
//     ft_strlcat(left_pfx, "     │    ", sizeof(left_pfx));
//     if (tree->left)
//         print_tree_recursive(tree->left, left_pfx);
//     else
//         printf("%s└─── NULL\n", left_pfx);

//     // right (le dernier élément a un `└─── right` au lieu de `├───`)
//     printf("%s     └─── right\n", prefix);
//     ft_strlcpy(right_pfx, prefix, sizeof(right_pfx));
//     ft_strlcat(right_pfx, "          ", sizeof(right_pfx));
//     if (tree->right)
//         print_tree_recursive(tree->right, right_pfx);
//     else
//         printf("%s└─── NULL\n", right_pfx);
// }

// void	print_tree(t_tree *tree)
// {
//     printf("├─── parsing\n");
//     if (tree)
//         print_tree_recursive(tree, "│    ");
//     else
//         printf("│    └─── NULL\n");

//     // Pseudo bloc d'erreur demandé
//     printf("└─── error\n     ├─── code: MINICODE_NONE\n     └─── msg: [No error.]\n");
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

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	if (tree->av)
		free_split(tree->av);
	if (tree)
		free(tree);
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
	else
		tree->parse_type = WORD_PARS;
}

int	value_from_list_to_tree(t_tree *branch, t_token **cmd, size_t count)
{
	size_t	i;
	t_token	*current;
	char	*tmp;

	i = 0;
	current = *cmd;
	while (i < count && current)
	{
		if (current->type == WORD && current->next
			&& current->next->type == DOUBLE && i + 1 < count)
		{
			tmp = ft_strjoin(current->value, current->next->value);
			if (!tmp)
				return (free_split(branch->av), 1);
			branch->av[i] = tmp;
			current = current->next->next;
			i += 2;
		}
		else
		{
			branch->av[i] = ft_strdup(current->value);
			if (!branch->av[i])
				return (free_split(branch->av), 1);
			current = current->next;
			i++;
		}
	}
	branch->av[i] = NULL;
	return (0);
}

t_tree	*left_branch(t_tree *tree, t_token **cmd, size_t count)
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
		return (free_split(left->av), free(left), free_tree(tree), NULL);
	return (left);
}

bool	search_pipe(t_token **cmd, size_t *count)
{
	t_token	*current;

	*count = 0;
	current = *cmd;
	while (current)
	{
		if (current->type == PIPE)
			return (true);
		current = current->next;
		(*count)++;
	}
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
		return (free_split(branch_pipe->av), free(branch_pipe),
			free_tree(tree), NULL);
	branch_pipe->av[1] = NULL;
	branch_pipe->fds = 0;
	branch_pipe->left = NULL;
	branch_pipe->right = NULL;
	return (branch_pipe);
}

t_tree	*no_pipe_tree(t_tree *tree, t_token **cmd, size_t *count)
{
	t_tree	*branch_pipe;

	branch_pipe = malloc(sizeof(t_tree));
	if (!branch_pipe)
		return (ft_putstr_fd("Error malloc\n", 2), NULL);
	branch_pipe->ac = *count;
	branch_pipe->parse_type = NO_PIPE_PARS;
	branch_pipe->av = malloc(sizeof(char *) * (*count + 1));
	if (!branch_pipe->av)
		return (free(branch_pipe), free_tree(tree), NULL);
	if (value_from_list_to_tree(branch_pipe, cmd, *count))
		return (free_split(branch_pipe->av), free(branch_pipe), NULL);
	branch_pipe->fds = 0;
	branch_pipe->left = NULL;
	branch_pipe->right = NULL;
	return (branch_pipe);
}

t_token	*new_head_actualisation(t_token **head, size_t count)
{
	t_token	*new_head;
	size_t	i;

	i = 0;
	new_head = *head;
	while (i < count)
	{
		new_head = new_head->next;
		i++;
	}
	return (new_head->next);
}

int	join_two_token(t_token *t1, t_token *t2, t_token *t3)
{
	t_token	*to_delete;
	char	*new_value;

	new_value = ft_strjoin(t1->value, t2->value);
	if (!new_value)
		return (1);
	free(t1->value);
	t1->value = ft_strdup(new_value);
	free(t2->value);
	to_delete = t2;
	t1->next = t3;
	free(to_delete);
	free(new_value);
	return (0);
}

int	check_post_redir(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if ((current->type == INREDIR && current->next
				&& current->next->type == INREDIR)
			|| (current->type == OUTREDIR && current->next
				&& current->next->type == OUTREDIR))
		{
			if (join_two_token(current, current->next, current->next->next))
				return (1);
		}
		if (current->type == OUTREDIR || current->type == INREDIR
			|| current->type == HEREDOC || current->type == APPOUTREDIR)
		{
			current = current->next;
			while (current->type == ESPACE)
				current = current->next;
			if (current->type != WORD)
				return (printf("syntax error near unexpected token '%s'\n",
						current->value), 2);
		}
	}
	return (0);
}

int	join_word_to_dbl_quote(t_token **head)
{
	t_token	*current;
	int		result_post_redir;

	current = (*head);
	while (current)
	{
		if (current->type == WORD && current->next
			&& current->next->type == DOUBLE)
		{
			if (join_two_token(current, current->next, current->next->next))
				return (1);
		}
		if (current->type == OUTREDIR || current->type == INREDIR
			|| current->type == HEREDOC || current->type == APPOUTREDIR)
		{
			result_post_redir = check_post_redir(current);
			if (result_post_redir)
				return (result_post_redir);
		}
		if (current->next == NULL && current->type == PIPE)
			return (printf("A pipe cannot finish a prompt\n"), 2);
		current = current->next;
	}
	return (0);
}

t_tree	*parser(t_token **cmd)
{
	size_t	count;
	t_token	*new_head;
	t_tree	*tree;

	count = 0;
	tree = new_pipe(NULL, cmd, &count);
	if (!tree)
	{
		tree = no_pipe_tree(NULL, cmd, &count);
		if (!tree)
			return (free_tree(tree), NULL);
		return (tree);
	}
	tree->left = left_branch(tree, cmd, count);
	if (!(tree)->left)
		return (free_tree(tree), NULL);
	new_head = new_head_actualisation(cmd, count);
	if (new_head)
	{
		tree->right = parser(&new_head);
		if (!tree->right)
			return (free_tree(tree), NULL);
	}
	return (tree);
}

int	lexer(t_tree **tree, char *line)
{
	t_token	*cmd;
	int		return_pars_line;
	int		return_trim_cmd;

	cmd = NULL;
	return_pars_line = parse_line(&cmd, line);
	if (return_pars_line == 2)
		return (clear_actual_command(&cmd), free_tree(*tree), 0);
	if (return_pars_line != 0)
		return (clear_actual_command(&cmd), 1);
	return_trim_cmd = join_word_to_dbl_quote(&cmd);
	if (return_trim_cmd == 1)
		return (clear_actual_command(&cmd), 1);
	if (return_trim_cmd == 2)
		return (clear_actual_command(&cmd), free_tree(*tree), 0);
	(*tree) = parser(&cmd);
	if (!*tree)
		return (clear_actual_command(&cmd), 2);
	clear_actual_command(&cmd);
	/* print_tree(*tree); */
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
