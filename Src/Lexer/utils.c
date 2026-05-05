/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:25:10 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/05 17:31:03 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char	*ft_stthreejoin(char const *s1, char const *s2, char const *s3)
{
	char	*result;
	int		i;
	int		j;
	int		k;
	int		size;

	size = ft_strlen((char *)s1) + ft_strlen((char *)s2) + ft_strlen((char *)s2) - 2;
	result = malloc(sizeof(char) * (size + 1));
	if (!result)
		return (NULL);
	i = 0 ;
	j = 0;
    k = 1;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	while (s2[j])
		result[i++] = s2[j++];
	while (s3[k] != 34)
		result[i++] = s2[k++];
	result[i] = '\0';
	return (result);
}
