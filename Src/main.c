/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:48:12 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/11 11:58:12 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	boucle_str(t_token **head)
{
	t_token	*current;

	current = *head;
	while (current)
	{
		printf ("valeur de la boucle :[%d, %s]\n", current->type, current->value);
		current = current->next;
	}
}

static void	print_node_type(t_pars_type type)
{
    switch (type)
    {
        case PIPE_PARS:			printf("PIPE_PARS\n"); break;
        case NO_PIPE_PARS:		printf("NO_PIPE_PARS\n"); break;
        case INREDIR_PARS:		printf("INREDIR_PARS\n"); break;
        case OUTREDIR_PARS:		printf("OUTREDIR_PARS\n"); break;
        case HEREDOC_PARS:		printf("HEREDOC_PARS\n"); break;
        case APPOUTREDIR_PARS:	printf("APPOUTREDIR_PARS\n"); break;
        case WORD_PARS:			printf("WORD_PARS\n"); break;
        case WORD_QUOTE_PARS:	printf("WORD_QUOTE_PARS\n"); break;
        default:				printf("UNKNOWN\n");
    }
}

static void	print_tree_argv(t_tree *tree)
{
    if (!tree->av || tree->ac == 0)
    {
        printf("NULL\n");
        return;
    }
    printf("[");
    for (int i = 0; i < tree->ac; i++)
    {
        if (tree->av[i])
            printf("[%s]", tree->av[i]);
    }
    printf("]\n");
}

static void	print_tree_recursive(t_tree *tree, char *prefix)
{
    char left_pfx[1024];
    char right_pfx[1024];
    char child_pfx[1024];

    ft_strlcpy(child_pfx, prefix, sizeof(child_pfx));
    ft_strlcat(child_pfx, "│    ", sizeof(child_pfx));

    // Type
    printf("%s└─── type: ", prefix);
    print_node_type(tree->parse_type);

    // Argc
    printf("%s     ├─── argc: %d\n", prefix, tree->ac);

    // Argv
    printf("%s     ├─── argv: ", prefix);
    print_tree_argv(tree);

    // fds
    printf("%s     ├─── fds\n%s     │    └─── %s\n", prefix, prefix, tree->fds ? "YES" : "NULL");

    // subshell
    printf("%s     ├─── subshell\n%s     │    └─── NULL\n", prefix, prefix); // Fixe à NULL pour l'instant

    // left
    printf("%s     ├─── left\n", prefix);
    ft_strlcpy(left_pfx, prefix, sizeof(left_pfx));
    ft_strlcat(left_pfx, "     │    ", sizeof(left_pfx));
    if (tree->left)
        print_tree_recursive(tree->left, left_pfx);
    else
        printf("%s└─── NULL\n", left_pfx);

    // right (le dernier élément a un `└─── right` au lieu de `├───`)
    printf("%s     └─── right\n", prefix);
    ft_strlcpy(right_pfx, prefix, sizeof(right_pfx));
    ft_strlcat(right_pfx, "          ", sizeof(right_pfx));
    if (tree->right)
        print_tree_recursive(tree->right, right_pfx);
    else
        printf("%s└─── NULL\n", right_pfx);
}

void	print_tree(t_tree *tree)
{
    printf("├─── parsing\n");
    if (tree)
        print_tree_recursive(tree, "│    ");
    else
        printf("│    └─── NULL\n");

    // Pseudo bloc d'erreur demandé
    printf("└─── error\n     ├─── code: MINICODE_NONE\n     └─── msg: [No error.]\n");
} */

int	main(int ac, char **av, char **env)
{
	char				*line;
	struct sigaction	action;
	t_tree				*tree;

	(void)ac;
	(void)av;
	(void)env;
	ft_memset(&action, 0, sizeof(action));
	tree = NULL;
	action.sa_handler = handler;
	while (1)
	{
		line = get_line();
		if (!line)
		{
			printf("exit");
			rl_clear_history();
			exit (0);
		}
		sigaction(SIGINT, &action, NULL);
		signal(SIGQUIT, SIG_IGN);
		if (lexer(&tree, line))
		{
			free(line);
			free_tree(tree);
			return (1);
		}
        
		free(line);
	}
	return (0);
}

