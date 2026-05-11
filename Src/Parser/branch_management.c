/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:21:50 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/11 15:03:05 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// int	lexer(t_tree *tree, char *line)
// {
// 	t_token	*cmd;
// 	int		return_pars_line;
// 	int		return_trim_cmd;

// 	cmd = NULL;
// 	return_pars_line = parse_line(&cmd, line);
// 	if (return_pars_line == 2)
// 		return (clear_actual_command(&cmd), free_tree(tree), 0);
// 	if (return_pars_line != 0)
// 		return (clear_actual_command(&cmd), 1);
// 	return_trim_cmd = join_word_to_dbl_quote(&cmd);
// 	if (return_trim_cmd == 1)
// 		return (clear_actual_command(&cmd), 1);
// 	if (return_trim_cmd == 2)
// 		return (clear_actual_command(&cmd), free_tree(tree), 0);
// 	(tree) = parser(&cmd);
// 	if (!tree)
// 		return (clear_actual_command(&cmd), 2);
// 	clear_actual_command(&cmd);
// 			// print_tree(sh->ast, 0);
// 	free_tree(tree);
// 	tree = NULL;
// 	return (0);
// }
