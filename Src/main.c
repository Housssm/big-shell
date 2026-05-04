/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:47 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:35:47 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*************************** TEMPORAIRE ************************/

void	lexer(t_token **cmd, char *line)
{
	if (parse_line(cmd, line))
		return (clear_actual_command(cmd));
	boucle_str(cmd);
	clear_actual_command(cmd);
}

int main(int ac, char **av, char **envp)
{
	if (ac < 2)
	{
		ft_printf("Error\n");
		return (1);
	}
    t_env   *env;

    env = init_env(envp);
    if (!ft_strcmp(av[1], "cd"))
		ft_cd(av + 2, &env);
	else if (!ft_strcmp(av[1], "echo"))
		ft_echo(av);
	else if (!ft_strcmp(av[1], "export"))
        ft_export(av + 2, &env);
	else if (!ft_strcmp(av[1], "pwd"))
		ft_pwd();
    return (0);
}

/*
int	main(void)
{
	char					*line;
	struct sigaction		action;
	t_token					*cmd;

	ft_memset(&action, 0, sizeof(action));
	cmd = NULL;
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
		lexer(&cmd, line); fonctoin qui retourne une structure 
		excec(tree)
		free(line);
	}
	return (0);
}*/
