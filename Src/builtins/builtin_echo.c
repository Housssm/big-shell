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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

int is_flag(char *str)
{
    int i;

    i = 0;
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

    i = 1;
    newline = 1;
    if (av[i] && is_flag(av[i]))
    {
        newline = 0;
        i++;
    }
    while (av[i])
    {
        ft_putstr(av[i]);
        if (av[i + 1])
            ft_putchar(' ');
        i++;
    }
    if (newline)
        ft_putchar('\n');
    return (0);
}

int main(int ac, char **av)
{
    ft_echo(av);
    return (0);
}