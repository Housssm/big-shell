/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:21:50 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/15 21:00:11 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_tree	*left_branch(t_tree *tree, t_token **cmd, size_t count)
// {
// 	t_tree	*left;

// 	left = malloc(sizeof(t_tree));
// 	if (!left)
// 		return (free_tree(tree), NULL);
// 	left->av = malloc(sizeof(char *) * (count + 1));
// 	if (!left->av)
// 		return (free(left), free_tree(tree), NULL);
// 	left->ac = count;
// 	left->fds = 0;
// 	left->left = NULL;
// 	left->right = NULL;
// 	what_is_ptype(left, ((*cmd)));
// 	if (value_from_list_to_tree(left, cmd, count))
// 		return (free_split(left->av), free(left), free_tree(tree), NULL);
// 	return (left);
// }


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


/*compte les arg*/


static int	is_redir_type(t_type type)
{
	return (type == INREDIR || type == OUTREDIR
		|| type == HEREDOC || type == APPOUTREDIR);
}

int	count_args(t_token *cmd)
{
	int		count;
	t_token	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		if (is_redir_type(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		if (current->type == WORD)
			count++;
		current = current->next;
	}
	return (count);
}


/*REMPLI STRUCTURE DE REDIR */

static t_lst_fd	*new_fd_node(char *file, int type)
{
	t_lst_fd	*node;

	node = malloc(sizeof(t_lst_fd));
	if (!node)
		return (NULL);
	node->file = ft_strdup(file);
	node->fd = -1;
	node->type = type;
	node->previous = NULL;
	node->next = NULL;
	return (node);
}

static void	add_fd_to_list(t_lst_fd **lst, t_lst_fd *new)
{
	t_lst_fd	*tmp;

	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->previous = tmp;
	}
}

t_lst_fd	*fill_fd_list(t_token **cmd)
{
	t_lst_fd	*fds;
	t_token		*current;
	t_lst_fd	*new;

	fds = NULL;
	current = *cmd;
	while (current)
	{
		if (current->type == INREDIR || current->type == OUTREDIR
			|| current->type == HEREDOC || current->type == APPOUTREDIR)
		{
			if (!current->next || current->next->type != WORD)
				return (NULL);
			new = new_fd_node(current->next->value, current->type);
			if (!new)
				return (NULL);
			add_fd_to_list(&fds, new);
			current = current->next;
		}
		current = current->next;
	}
	return (fds);
}



/*********REMPLI AVEC ARG UNIQUEMENT  */

#include "../minishell.h"

// static int	is_redir_type(t_type type)
// {
// 	return (type == INREDIR || type == OUTREDIR
// 		|| type == HEREDOC || type == APPOUTREDIR);
// }

int	fill_argv(t_tree *branch, t_token **cmd, int arg_count)
{
	int		i;
	t_token	*current;

	i = 0;
	current = *cmd;
	while (current && i < arg_count)
	{
		if (is_redir_type(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		if (current->type == WORD)
		{
			branch->av[i] = ft_strdup(current->value);
			if (!branch->av[i])
				return (1);
			i++;
		}
		current = current->next;
	}
	branch->av[i] = NULL;
	return (0);
}



t_tree	*left_branch(t_tree *tree, t_token **cmd, size_t count)
{
	t_tree	*left;
	int		arg_count;
	(void)count;
	arg_count = count_args(*cmd);
	left = malloc(sizeof(t_tree));
	if (!left)
		return (free_tree(tree), NULL);
	left->av = malloc(sizeof(char *) * (arg_count + 1));
	if (!left->av)
		return (free(left), free_tree(tree), NULL);
	left->ac = arg_count;
	left->fds = fill_fd_list(cmd);
	left->left = NULL;
	left->right = NULL;
	what_is_ptype(left, (*cmd));
	if (fill_argv(left, cmd, arg_count))
		return (free_split(left->av), free(left), free_tree(tree), NULL);
	return (left);
}


t_tree	*no_pipe_tree(t_tree *tree, t_token **cmd, size_t *count)
{
	t_tree	*branch;
	int		arg_count;

	(void)count;
	arg_count = count_args(*cmd);
	branch = malloc(sizeof(t_tree));
	if (!branch)
		return (ft_putstr_fd("Error malloc\n", 2), NULL);
	branch->ac = arg_count;
	branch->parse_type = NO_PIPE_PARS;
	branch->av = malloc(sizeof(char *) * (arg_count + 1));
	if (!branch->av)
		return (free(branch), free_tree(tree), NULL);
	branch->fds = fill_fd_list(cmd);
	if (fill_argv(branch, cmd, arg_count))
		return (free_split(branch->av), free(branch), NULL);
	branch->left = NULL;
	branch->right = NULL;
	return (branch);
}






// t_tree	*no_pipe_tree(t_tree *tree, t_token **cmd, size_t *count)
// {
// 	t_tree	*branch_pipe;

// 	branch_pipe = malloc(sizeof(t_tree));
// 	if (!branch_pipe)
// 		return (ft_putstr_fd("Error malloc\n", 2), NULL);
// 	branch_pipe->ac = *count;
// 	branch_pipe->parse_type = NO_PIPE_PARS;
// 	branch_pipe->av = malloc(sizeof(char *) * (*count + 1));
// 	if (!branch_pipe->av)
// 		return (free(branch_pipe), free_tree(tree), NULL);
// 	if (value_from_list_to_tree(branch_pipe, cmd, *count))
// 		return (free_split(branch_pipe->av), free(branch_pipe), NULL);
// 	branch_pipe->fds = 0;
// 	branch_pipe->left = NULL;
// 	branch_pipe->right = NULL;
// 	return (branch_pipe);
// }







static t_tree	*build_ast(t_token **cmd)
{
	size_t	count;
	t_token	*new_head;
	t_tree	*tree;

	count = 0;
	tree = new_pipe(NULL, cmd, &count);
	if (!tree)
	{
		tree = no_pipe_tree(NULL, cmd, &count);
		return (tree);
	}
	tree->left = left_branch(tree, cmd, count);
	if (!tree->left)
		return (free_tree(tree), NULL);
	new_head = new_head_actualisation(cmd, count);
	if (new_head)
	{
		tree->right = build_ast(&new_head);
		if (!tree->right)
			return (free_tree(tree), NULL);
	}
	return (tree);
}

t_tree	*parser(t_token **cmd)
{
	return (build_ast(cmd));
}

int	parser_and_execute(t_token **cmd, t_env **env, int *last_status)
{
	t_tree	*tree;

	tree = build_ast(cmd);
	if (!tree)
		return (1);
	shell_execute(tree, env, last_status);
	free_tree(tree);
	return (0);
}
