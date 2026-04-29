/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtodzzi <mtodzzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 13:44:48 by mtodzzi           #+#    #+#             */
/*   Updated: 2026/04/28 13:44:48 by mtodzzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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

int ft_pwd(void)
{
    char    cwd[PATH_MAX];

    while (!getcwd(cwd, PATH_MAX))
    {
        ft_putstr("pwd: error retrieving current directory\n");
        return (1);
    }
    ft_putstr(cwd);
    ft_putchar('\n');
    return (0);

}

int main(int ac, char **av)
{
    ft_pwd();
    return (0);
}