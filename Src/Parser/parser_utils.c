/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:27:37 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/07 15:28:40 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	i = 0;
	current = *cmd;
	while (i < count && current)
	{
		branch->av[i] = ft_strdup(current->value);
		if (!branch->av[i])
			return (free_split(branch->av), 1);
		current = current->next;
		i++;
	}
	branch->av[i] = NULL;
	return (0);
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
