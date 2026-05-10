/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 10:30:20 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/05/04 10:30:20 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            ft_printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return (0);
}
