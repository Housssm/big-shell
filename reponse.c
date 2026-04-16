#include "Src/minishell.h"

/*
 * Correction principale:
 * - allouer value avant ft_memcpy
 * - terminer la chaine avec '\0'
 * - corriger les conditions de parcours des espaces et des mots
 */
size_t	parse_line(t_token *cmd, char *line)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*value;
	t_token	*cursor;

	i = 0;
	len = 0;
	cursor = cmd;
	while (cursor->next)
		cursor = cursor->next;
	while (line[i])
	{
		while (line[i] && ((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			i++;
		if (!line[i])
			break ;
		j = i;
		while (line[i] && !((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			i++;
		value = malloc((i - j) + 1);
		if (!value)
			return (1);
		ft_memcpy(value, &line[j], i - j);
		value[i - j] = '\0';
		len = add_node(&cursor, value);
		free(value);
		if (len != 0)
			return (len);
		if (cursor->next)
			cursor = cursor->next;
	}
	return (0);
}
