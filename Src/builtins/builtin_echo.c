/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 12:54:38 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/04/29 12:54:38 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int  is_flag(char *str)
{
    int i;

    i = 1;
    if (str[0] != '-')
        return (0);
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (i > 1);
}

int    ft_echo(char **av)
{
    int i;
    int newline;

    i = 2;
    newline = 1;
    if (av[i] && is_flag(av[i]))
    {
        newline = 0;
        i++;
    }
    while (av[i])
    {
        ft_printf("%s", av[i]);
        if (av[i + 1])
            ft_printf(" ");
        i++;
    }
    if (newline)
        ft_printf("\n");
    return (0);
}
/*
int main(int ac, char **av)
{
    ft_echo(av);
    return (0);
}*/