/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 10:20:58 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/05/04 10:20:58 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void    remove_node(t_env **env, t_env *current, t_env *prev)
{
    if (prev)
        prev->next = current->next;
    else
        *env = current->next;
    free(current->key);
    free(current->value);
    free(current);
}

static void    find_and_remove(t_env **env, char *key)
{
    t_env   *current;
    t_env   *prev;

    current = *env;
    prev = NULL;
    while (current)
    {
        if (!strcmp(current->key, key))
        {
            remove_node(env, current, prev);
            return ;
        }
        prev = current;
        current = current->next;
    }
}

int    ft_unset(char **av, t_env **env)
{
    int     i;

    i = 1;
    while (av[i])
    {
        find_and_remove(env, av[i]);
        i++;
    }
    return (0);
}