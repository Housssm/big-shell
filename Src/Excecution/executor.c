/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:00:00 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/14 15:00:00 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exec_pipe(t_tree *node, t_env **env, int *last_status);
static int	exec_cmd(t_tree *node, t_env **env, int *last_status);

int	shell_execute(t_tree *tree, t_env **env, int *last_status)
{
    if (!tree)
        return (0);
    if (tree->parse_type == PIPE_PARS)
        return (exec_pipe(tree, env, last_status));
    else
        return (exec_cmd(tree, env, last_status));
}

static int	exec_pipe(t_tree *node, t_env **env, int *last_status)
{
    int		pipe_fd[2];
    pid_t	left_pid;
    pid_t	right_pid;

    if (pipe(pipe_fd) == -1)
        return (perror("pipe"), 1);
    left_pid = fork();
    if (left_pid == -1)
        return (perror("fork"), 1);
    if (left_pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        exit(shell_execute(node->left, env, last_status));
    }
    right_pid = fork();
    if (right_pid == -1)
        return (perror("fork"), 1);
    if (right_pid == 0)
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        exit(shell_execute(node->right, env, last_status));
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, last_status, 0);
    return (WEXITSTATUS(*last_status));
}

static int	exec_cmd(t_tree *node, t_env **env, int *last_status)
{
    char	*path;
    char	**envp;
    pid_t	pid;

    if (is_builtin(node->av[0]))
        return (run_builtin(node, env, last_status));
    path = get_cmd_path(node->av[0], *env);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(node->av[0], 2);
        ft_putstr_fd("\n", 2);
        return (127);
    }
    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    if (pid == 0)
    {
        envp = env_to_tab(*env);
        execve(path, node->av, envp);
        free(path);
        free_split(envp);
        perror("execve");
        exit(126);
    }
    waitpid(pid, last_status, 0);
    free(path);
    return (WEXITSTATUS(*last_status));
}
