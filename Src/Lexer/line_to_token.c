/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_to_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:52 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:35:53 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	new_token(char c, int *flag)
{
	size_t		i;
	char		*array;

	array = "|&<> \t";
	if (*flag != 0)
		return (0);
	i = 0;
	while (array[i])
	{
		if (array[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	handle_quote(t_token **cmd, char *line, int *i, int *in_quote)
{
	char	quote_char;
	int		start;

	quote_char = line[*i];
	add_node(cmd, line, *i, *i);
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (!line[*i])
		return (ft_putstr_fd("Unclosed quote\n", 2), 1);
	add_node(cmd, line, start, *i - 1);
	add_node(cmd, line, *i, *i);
	(*i)++;
	*in_quote = 0;
	return (0);
}

void	add_word_token(t_token **cmd, char *line, int start, int end)
{
	if (start < end)
		add_node(cmd, line, start, end - 1);
}

int	process_token(t_token **cmd, char *line, int *i, int *in_quote)
{
	int	start;

	start = *i;
	if (line[*i] == 34 || line[*i] == 39)
	{
		*in_quote = 1;
		return (handle_quote(cmd, line, i, in_quote));
	}
	while (line[*i] && !new_token(line[*i], in_quote))
		(*i)++;
	add_word_token(cmd, line, start, *i);
	return (0);
}

int	parse_line(t_token **cmd, char *line)
{
	size_t	i;
	int		in_quote;

	i = 0;
	in_quote = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (process_token(cmd, line, (int *)&i, &in_quote))
			return (1);
		if (line[i] && !in_quote && new_token(line[i], &in_quote))
		{
			add_node(cmd, line, i, i);
			i++;
		}
	}
	return (0);
}
