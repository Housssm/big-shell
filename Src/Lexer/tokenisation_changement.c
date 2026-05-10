/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation_changement.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:25:10 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/07 15:34:47 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
