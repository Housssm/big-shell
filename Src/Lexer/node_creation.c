/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:49 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/29 12:14:38 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_add_last(t_token *head)
{
	t_token	*temp;

	if (!head)
		return (NULL);
	temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	define_cmd(t_token *cmd, t_type opcode)
{
	cmd->type = opcode;
	cmd->next = NULL;
}

void	what_is_it(t_token *cmd, char *str)
{
	if (str[0] == '|')
		define_cmd(cmd, PIPE);
	else if (str[0] == '<' && !str[1])
		define_cmd(cmd, INREDIR);
	else if (str[0] == '>' && !str[1])
		define_cmd(cmd, OUTREDIR);
	else if (str[0] == '<' && str[1] == '<')
		define_cmd(cmd, HEREDOC);
	else if (str[0] == '>' && str[1] == '>')
		define_cmd(cmd, APPOUTREDIR);
	else if (str[0] == 34)
		define_cmd(cmd, WORD);
	else if (str[0] == 39)
		define_cmd(cmd, WORD);
	else
		define_cmd(cmd, WORD);
}

t_token	*create_token_from_line(char *line, int beg, int end)
{
	char	*value;
	t_token	*token;

	value = malloc(sizeof(char) * (end - beg + 2));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &line[beg], end - beg + 2);
	if (!is_blank(value))
		return (free(value), NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(value), ft_putstr_fd("Unclosed quote\n", 2), NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (free(value), free(token), NULL);
	what_is_it(token, token->value);
	token->next = NULL;
	free(value);
	return (token);
}

int	add_node(t_token **head, char *line, int beg, int end)
{
	t_token	*new_token;

	new_token = create_token_from_line(line, beg, end);
	if (!new_token)
		return (1);
	if (!*head)
		*head = new_token;
	else
		ft_add_last(*head)->next = new_token;
	return (0);
}
