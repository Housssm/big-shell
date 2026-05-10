/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:35:58 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/04/29 13:35:58 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int    is_valid_key(char *str)
{
    int i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static void    print_export(t_env *env)
{
    while (env)
    {
        ft_printf("declare -x ");
        ft_printf("%s", env->key);
        if (env->value)
            ft_printf("=\"%s\"", env->value);
        ft_printf("\n");
        env = env->next;
    }
}

int    ft_export(char **av, t_env **env)
{
    int     i;
    char    *equal;
    char    *key;
    char    *value;

    if (!av[0])
    {
        print_export(*env);
        return (0);
    }
    i = 0;
    while (av[i])
    {
        if (!is_valid_key(av[i]))
        {
            ft_printf("export: not a valid identifier: %s\n", av[i]);
            i++;
            continue ;
        }
        equal = ft_strchr(av[i], '=');
        if (equal)
        {
            key = ft_strndup(av[i], equal - av[i]);
            value = ft_strdup(equal + 1);
            update_env(env, key, value);
            free(key);
            free(value);
        }
        else
            update_env(env, av[i], NULL); // export sans valeur
        i++;
    }
    return (0);
}