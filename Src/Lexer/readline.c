/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:58 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:50:46 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal_received = 0; // save lst msg  

char	*get_line(void)
{
	char	*line_read;

	line_read = (char *) NULL;
	line_read = readline("shellinho:~$ ");
	if (line_read && *line_read)
		add_history(line_read);
	return (line_read);
}

void	handler(int sigtype)
{
	if (sigtype == SIGINT)
	{
		g_signal_received = sigtype;
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
	}
}
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

