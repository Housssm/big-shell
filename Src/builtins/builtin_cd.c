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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct s_env
{
    char    *key;   /* PATH */
    char    *value;   /* /usr/bin:/bin */
    struct s_env *next;
}   t_env;

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

void    ft_putstr(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        ft_putchar(str[i]);
        i++;
    }
}
/* parcour la liste et retourne la valeur associé*/
char    *get_env_value(t_env *env, char *key)
{
    while (env)
    {
        if (!strcmp(env->key, key))
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
        if (!strcmp(current->key, key))
        {
            free(current->value);
            current->value = strdup(value);
            return (0);
        }
        current = current->next;
    }
    new = malloc(sizeof(t_env)); // si la clé n'existe pas
    if (!new)
        return (1);
    new->key = strdup(key);
    new->value = strdup(value);
    new->next = *env;
    *env = new;
    return (0);
}

t_env   *init_env(char **envp)
{
    t_env   *node;
    t_env   *head;
    char    *sep;
    int i;

    head = NULL;
    i = 0;
    while (envp[i])
    {
        node = malloc(sizeof(t_env));
        if (!node)
            return (NULL);
        sep = strchr(envp[i], '=');
        node->key = strndup(envp[i], sep - envp[i]);
        node->value = strdup(sep + 1);
        node->next = head;
        head = node;
        i++;
    }
    return (head);
}

char    *cd_case(char **av, t_env *env)
{
    char    *path;

    if (!av[1]) // Cas 1 : sans argument
    {
        path = get_env_value(env, "HOME");
        if (!path)
        {
            ft_putstr("cd: HOME not set\n");
            return (NULL);
        }
    }
    else if (!strcmp(av[1], "-")) // Cas 2 : cd - 
    {
        path = get_env_value(env, "OLDPWD");
        if (!path)
        {
            ft_putstr("cd: OLDPWD not set\n");
            return (NULL);
        }
        ft_putstr(path);
        ft_putchar('\n');
    }
    else
        path = av[1]; // Cas 3 : cd chemin
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
        ft_putstr("cd: ");
        ft_putstr(path);
        ft_putstr(": No such file or directory\n");
        return (1);
    }
    update_env(env, "OLDPWD", cwd); // Mettre à jour OLDPWD et PWD dans l'environnement
    getcwd(cwd, PATH_MAX);
    update_env(env, "PWD", cwd);
    return (0);
}

int main(int ac, char **av, char **envp)
{
    t_env   *env;

    env = init_env(envp);
    ft_cd(av, &env);
    return (0);
}
