/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:36:47 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:38:21 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_blank(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '	')
			return (1);
		i++;
	}
	return (0);
}

void	lexer(t_token **cmd, char *line)
{
	if (parse_line(cmd, line))
		return (clear_actual_command(cmd));
	boucle_str(cmd);
	clear_actual_command(cmd);
}
