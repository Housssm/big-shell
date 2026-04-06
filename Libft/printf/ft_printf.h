/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:39:29 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/06 15:44:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include   <unistd.h>
# include   <stdio.h>
# include   <stdlib.h>
# include   <string.h>
// # include   <bsd/string.h>
# include   <stdint.h>
# include   <stdarg.h>
# include   <limits.h>

int	ft_print_char(char c);
int	ft_print_percent(char c);
int	ft_print_dec(int nb);
int	ft_print_unsigned(unsigned int nb);
int	ft_print_hexa_upper(unsigned long int nb);
int	ft_print_hexa_lower(unsigned long int nb);
int	ft_print_adress(void *ptr);
int	ft_print_str(char *str);
int	ft_printf(const char *str, ...);

#endif