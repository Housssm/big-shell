/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:03:16 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/04/27 11:03:16 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* parcour la liste et retourne la valeur associé*/
static char    *get_env_value(t_env *env, char *key)
{
    while (env)
    {
        if (!ft_strcmp(env->key, key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

int update_env(t_env **env, char *key, char *value)
{
    t_env   *current;
    t_env   *new;

    current = *env;
    while (current)// si la clé exite déja 
    {
        if (!ft_strcmp(current->key, key))
        {
            free(current->value);
            current->value = ft_strdup(value);
            return (0);
        }
        current = current->next;
    }
    new = malloc(sizeof(t_env)); // si la clé n'existe pas
    if (!new)
        return (1);
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = *env;
    *env = new;
    return (0);
}


// t_env   *init_env(char **envp)
// {
//     t_env   *node;
//     t_env   *head;
//     char    *sep;
//     int i;

//     head = NULL;
//     i = 0;
//     while (envp[i])
//     {
//         node = malloc(sizeof(t_env));
//         if (!node)
//             return (NULL);
//         sep = ft_strchr(envp[i], '=');
//         node->key = ft_strndup(envp[i], sep - envp[i]);
//         node->value = ft_strdup(sep + 1);
//         node->next = head;
//         head = node;
//         i++;
//     }
//     return (head);
// }
void    free_env(t_env *head)
{
    t_env *current;
    t_env *next;

    if (!head)
        return;
    current = head;
    while (current)
    {
		next = current->next;
        if (current->key)
            free(current->key);
        if (current->value)
            free(current->value);
		free(current);
		current = next;
    }
}



t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	char	*sep;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (free_env(head), NULL);
		sep = ft_strchr(envp[i], '=');
		new_node->key = ft_strndup(envp[i], sep - envp[i]);
        if (!new_node->key)
            return (free(new_node), free_env(head), NULL);
		new_node->value = ft_strdup(sep + 1);
        if(!new_node->value)
            return (free(new_node->key), free(new_node), free_env(head), NULL);
		new_node->next = NULL;
		if (head == NULL)
		{
			head = new_node;
			current = new_node;
		}
		else
		{
			current->next = new_node;
			current = new_node;
		}
		i++;
	}
	return (head);
}

static char    *cd_case(char **av, t_env *env)
{
    char    *path;

    if (!av[0]) // Cas 1 : sans argument
    {
        path = get_env_value(env, "HOME");
        if (!path)
        {
            ft_printf("cd: HOME not set\n");
            return (NULL);
        }
    }
    else if (!ft_strcmp(av[0], "-")) // Cas 2 : cd - 
    {
        path = get_env_value(env, "OLDPWD");
        if (!path)
        {
            ft_printf("cd: OLDPWD not set\n");
            return (NULL);
        }
        ft_printf("%s\n", path);
    }
    else
        path = av[0]; // Cas 3 : cd chemin
    return (path);
}

int    ft_cd(char **av, t_env **env)
{
    char    *path;
    char    cwd[PATH_MAX];

    getcwd(cwd, PATH_MAX); // Sauvegarder l'ancien répertoire avant de changer
    path = cd_case(av, *env);
    if (!path)
        return (1);
    if (chdir(path) == -1) // Effectuer le changement de répertoire
    {
        ft_printf("cd: %s : No such file or directory\n", path);
        return (1);
    }
    update_env(env, "OLDPWD", cwd); // Mettre à jour OLDPWD et PWD dans l'environnement
    getcwd(cwd, PATH_MAX);
    update_env(env, "PWD", cwd);
    return (0);
}
/*
int main(int ac, char **av, char **envp)
{
    t_env   *env;

    env = init_env(envp);
    ft_cd(av, &env);
    return (0);
}
*/