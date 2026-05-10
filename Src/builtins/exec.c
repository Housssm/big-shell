/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 16:41:32 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/05/10 16:41:32 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		ft_echo(av + 1);
	else if (!ft_strcmp(av[1], "export"))
        ft_export(av + 1, &env);
	else if (!ft_strcmp(av[1], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(av[1], "unset"))
		ft_unset(av + 1, &env);
	else if (!ft_strcmp(av[1], "env"))
		ft_env(env);
	else if (!ft_strcmp(av[1], "exit"))
		ft_exit(av + 1);
    return (0);
}