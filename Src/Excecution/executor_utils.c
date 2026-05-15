#include "../minishell.h"

t_env	*find_env_var(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}


// int	is_builtin(char *cmd)
// {
//     if (!cmd)
//         return (0);
//     if (ft_strcmp(cmd, "echo") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "cd") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "pwd") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "export") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "unset") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "env") == 0)
//         return (1);
//     if (ft_strcmp(cmd, "exit") == 0)
//         return (1);
//     return (0);
// }

int	run_builtin(t_tree *node, t_env **env, int *last_status)
{
    if (ft_strcmp(node->av[0], "echo") == 0)
        *last_status = ft_echo(node->av);
    else if (ft_strcmp(node->av[0], "cd") == 0)
        *last_status = ft_cd(node->av, env);
    else if (ft_strcmp(node->av[0], "pwd") == 0)
        *last_status = ft_pwd();
    else if (ft_strcmp(node->av[0], "export") == 0)
        *last_status = ft_export(node->av, env);
    else if (ft_strcmp(node->av[0], "unset") == 0)
        *last_status = ft_unset(node->av, env);
    else if (ft_strcmp(node->av[0], "env") == 0)
        *last_status = ft_env(*env);
    else if (ft_strcmp(node->av[0], "exit") == 0)
        *last_status = ft_exit(node->av, *env);
    return (*last_status);
}

char	**env_to_tab(t_env *env)
{
    t_env	*tmp;
    char	**tab;
    char	*tmp_str;
    int		i;

    i = 0;
    tmp = env;
    while (tmp && ++i)
        tmp = tmp->next;
    tab = malloc(sizeof(char *) * (i + 1));
    if (!tab)
        return (NULL);
    i = 0;
    tmp = env;
    while (tmp)
    {
        tmp_str = ft_strjoin(tmp->key, "=");
        tab[i] = ft_strjoin(tmp_str, tmp->value);
        free(tmp_str);
        i++;
        tmp = tmp->next;
    }
    tab[i] = NULL;
    return (tab);
}

static char	*find_in_path(char *cmd, char **paths)
{
    char	*path;
    char	*tmp;
    int		i;

    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(path, F_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return (NULL);
}

char	*get_cmd_path(char *cmd, t_env *env)
{
    char	**paths;
    char	*path_value;
    char	*full_path;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    path_value = NULL;
    while (env)
    {
        if (ft_strcmp(env->key, "PATH=") == 0)
            path_value = env->value;
        env = env->next;
    }
    if (!path_value)
        return (NULL);
    paths = ft_split(path_value, ':');
    full_path = find_in_path(cmd, paths);
    free_split(paths);
    return (full_path);
}
