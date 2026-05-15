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

// int main(int ac, char **av, char **envp)
// {
// 	if (ac < 2)
// 	{
// 		ft_printf("Error\n");
// 		return (1);
// 	}
//     t_env   *env;

//     env = init_env(envp);
//     if (!ft_strcmp(av[1], "cd"))
// 		ft_cd(av + 2, &env);
// 	else if (!ft_strcmp(av[1], "echo"))
// 		ft_echo(av + 1);
// 	else if (!ft_strcmp(av[1], "export"))
//         ft_export(av + 1, &env);
// 	else if (!ft_strcmp(av[1], "pwd"))
// 		ft_pwd();
// 	else if (!ft_strcmp(av[1], "unset"))
// 		ft_unset(av + 1, &env);
// 	else if (!ft_strcmp(av[1], "env"))
// 		ft_env(env);
// 	else if (!ft_strcmp(av[1], "exit"))
// 		ft_exit(av + 1);
//     return (0);
// }




int	is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

// int	run_builtin(t_tree *node, t_env **env, int *last_status)
// {
// 	(void)last_status;
//     if (ft_strcmp(node->av[0], "echo") == 0)
//         return (ft_echo(node->av));
//     if (ft_strcmp(node->av[0], "cd") == 0)
//         return (ft_cd(node->av, env));
//     if (ft_strcmp(node->av[0], "pwd") == 0)
//         return (ft_pwd());
//     if (ft_strcmp(node->av[0], "export") == 0)
//         return (ft_export(node->av, env));
//     if (ft_strcmp(node->av[0], "unset") == 0)
//         return (ft_unset(node->av, env));
//     if (ft_strcmp(node->av[0], "env") == 0)
//         return (ft_env(*env));
//     if (ft_strcmp(node->av[0], "exit") == 0)
//         return (ft_exit(node->av));
//     return (1);
// }

// int	run_builtin(t_tree *node, t_env **env, int *last_status)
// {
// 	(void)last_status;
// 	if (ft_strcmp(node->av[0], "echo") == 0)
// 		return (ft_echo(node->av));
// 	if (ft_strcmp(node->av[0], "cd") == 0)
// 		return (ft_cd(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "pwd") == 0)
// 		return (ft_pwd());
// 	if (ft_strcmp(node->av[0], "export") == 0)
// 		return (ft_export(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "unset") == 0)
// 		return (ft_unset(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "env") == 0)
// 		return (ft_env(*env));
// 	if (ft_strcmp(node->av[0], "exit") == 0)
// 		return (ft_exit(node->av));
// 	return (127);
// }