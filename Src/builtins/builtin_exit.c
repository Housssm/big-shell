/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 11:12:56 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/05/04 11:12:56 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int    ft_atoi_exit(char *str)
{
    long    res;
    int     sign;
    int     i;

    i = 0;
    sign = 1;
    res = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (ft_isdigit(str[i]))
    {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    return ((int)(res * sign));
}

static int    is_numeric(char *str)
{
    int     i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int    ft_exit(char **av, t_env *env)
{
    ft_printf("exit\n");
    if (!av[1])
        exit(0);
    if (!is_numeric(av[1]))
    {
        ft_printf("exit: numeric argument required\n");
        exit(2);
    }
    if (av[2])
    {
        ft_printf("exit: too many arguments\n");
        return (1);
    }
    free_env(env);
    exit(ft_atoi_exit(av[1]) % 256); // code de retour 0 - 256 
}