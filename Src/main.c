/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:48:12 by hoel-har          #+#    #+#             */
/*   Updated: 2026/05/15 16:08:22 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gere erreur de fin < > ou si nmauvaise synthaxe
// tout nettre dans le stdin sauf la sortie dans le OUT
// si cest pas nul je recule sinonn jexcetee
//echo hello >> file1 > file2 > file3 > file4
// la je cfais if echo ensuite redirection, boucle pour aller jusqua la derniere redir et jecris dans le fichier de fin, attention a bien ouvrir les fd des fichiers que jai parcourr

#include "minishell.h"

void	boucle_str(t_token **head)
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
}


// Dans Src/main.c

int	lexer(t_tree **tree, char *line, t_env **env, int *last_status)
{
    t_token	*cmd;
    int		return_pars_line;
    int		return_trim_cmd;
    (void)env;
    (void)last_status;
    cmd = NULL;
    return_pars_line = parse_line(&cmd, line);
    if (return_pars_line == 2)
        return (clear_actual_command(&cmd), 0); // Quitter proprement
    if (return_pars_line != 0)
        return (clear_actual_command(&cmd), 1); // Erreur de syntaxe
    return_trim_cmd = join_word_to_dbl_quote(&cmd);
    if (return_trim_cmd != 0)
        return (clear_actual_command(&cmd), return_trim_cmd);
    (*tree) = parser(&cmd);
    clear_actual_command(&cmd); // Nettoie la liste de tokens, on a l'arbre
    if (!*tree)
        return (2); // Erreur du parser
    print_tree(*tree);
    // // --- C'est ici que l'exécution est lancée ---
    // if (*tree)
    //     shell_execute(*tree, env, last_status);
    // // -------------------------------------------

    free_tree(*tree);
    *tree = NULL;
    return (0);
}

int	main(int ac, char **av, char **envp)
{
    char				*line;
    struct sigaction	action;
    t_tree				*tree;
    t_env				*env;
    int					last_status;

    (void)ac;
    (void)av;
    env = init_env(envp);
    last_status = 0;
    ft_memset(&action, 0, sizeof(action));
    tree = NULL;
    action.sa_handler = handler;
    while (1)
    {
        line = get_line();
        if (!line)
        {
            printf("exit\n");
            rl_clear_history();
            // free_env(&env); // Pense à créer une fonction pour ça
            exit (last_status);
        }
        sigaction(SIGINT, &action, NULL);
        signal(SIGQUIT, SIG_IGN);
        if (lexer(&tree, line, &env, &last_status))
        {
            // Gérer les erreurs de lexer/parser si nécessaire
        }
        free(line);
    }
    return (last_status);
}



// int	lexer(t_tree **tree, char *line)
// {
// 	t_token	*cmd;
// 	int		return_pars_line;
// 	int		return_trim_cmd;

// 	cmd = NULL;
// 	return_pars_line = parse_line(&cmd, line);
// 	if (return_pars_line == 2)
// 		return (clear_actual_command(&cmd), free_tree(*tree), 0);
// 	if (return_pars_line != 0)
// 		return (clear_actual_command(&cmd), 1);
// 	return_trim_cmd = join_word_to_dbl_quote(&cmd);
// 	if (return_trim_cmd == 1)
// 		return (clear_actual_command(&cmd), 1);
// 	if (return_trim_cmd == 2)
// 		return (clear_actual_command(&cmd), free_tree(*tree), 0);
// 	(*tree) = parser(&cmd);
// 	if (!*tree)
// 		return (clear_actual_command(&cmd), 2);
//     boucle_str(&cmd);
// 	clear_actual_command(&cmd);
// 	print_tree(*tree);


// //  rentrer fonctions ici
    
// 	free_tree(*tree);
// 	*tree = NULL;
// 	return (0);
// }



// int	main(int ac, char **av, char **env)
// {
// 	char				*line;
// 	struct sigaction	action;
// 	t_tree				*tree;

// 	(void)ac;
// 	(void)av;
// 	(void)env;
// 	ft_memset(&action, 0, sizeof(action));
// 	tree = NULL;
// 	action.sa_handler = handler;
// 	while (1)
// 	{
// 		line = get_line();
// 		if (!line)
// 		{
// 			printf("exit");
// 			rl_clear_history();
// 			exit (0);
// 		}
// 		sigaction(SIGINT, &action, NULL);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (lexer(&tree, line))
// 		{
// 			free(line);
// 			free_tree(tree);
// 			return (1);
// 		}
        
// 		free(line);
// 	}
// 	return (0);
// }

