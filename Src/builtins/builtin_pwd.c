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

#include "../minishell.h"

int ft_pwd(void)
{
    char    cwd[PATH_MAX];

    while (!getcwd(cwd, PATH_MAX))
    {
        ft_printf("pwd: error retrieving current directory\n");
        return (1);
    }
    ft_printf("%s\n", cwd);
    return (0);
}

/*
int main(int ac, char **av)
{
    ft_pwd();
    return (0);
}*/