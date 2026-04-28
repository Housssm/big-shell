/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoel-har <hoel-har@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:35:43 by hoel-har          #+#    #+#             */
/*   Updated: 2026/04/25 13:35:43 by hoel-har         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>

# include "../Libft/libft/libft.h"

typedef enum e_type
{
	PIPE,
	WORD,
	INREDIR,
	OUTREDIR,
	HEREDOC,
	APPOUTREDIR,
	MAIN,
	PROCESS,
	CHI
}	t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_parser_type
{
	PIPE_PARS,
	INREDIR_PARS,
	OUTREDIR_PARS,
	HEREDOC_PARS,
	APPOUTREDIR_PARS,
	WORD_PARS,
	WORD_QUOTE_PARS,

}	t_pars_type;

typedef struct s_lst_fd
{
	char			*file;
	int				fd;
	struct s_lst_fd	*previous;
	struct s_lst_fd	*next;
}	t_lst_fd;

typedef struct s_tree
{
	t_pars_type		parse_type;
	int				ac;
	char			**av;
	struct s_lst_fd	*fds;
	struct s_tree	*right;
	struct s_tree	*left;
}	t_tree;
/* // typedef struct s_parsing\
// {
// 	t_typepars	type;
// 	int			ac;
// 	char		**av;
// 	int			*fd;
// 	tree 		left;
// 	tree 		right;
// }	t_parsing;
 */

/* structure a envoye pour lecxec:
	-env
	-token// pas besoin si lexing
	lexing
	noeud suidvant
 */
typedef struct s_parse
{
	int	pos_first_quote;
	int	pos_sec_quote;
}	t_parse;

extern volatile sig_atomic_t	g_signal_received;
// var dt valeur pt etre mod en dehors progr -> assure portabilite general

// Lexer
char	*get_line(void);
void	handler(int sigtype);
int		new_token(char c, int *flag);
int		handle_quote(t_token **cmd, char *line, int *i, int *in_quote);
void	add_word_token(t_token **cmd, char *line, int start, int end);
int		process_token(t_token **cmd, char *line, int *i, int *in_quote);
int		parse_line(t_token **cmd, char *line);
t_token	*ft_add_last(t_token *head);
void	define_cmd(t_token *cmd, t_type opcode);
void	what_is_it(t_token *cmd, char *str);
t_token	*create_token_from_line(char *line, int beg, int end);
int		add_node(t_token **head, char *line, int beg, int end);
int		is_blank(char *str);
int		is_blank(char *str);
// void	lexer(t_tree *tree, char *line);

/*************************** TEMPORAIRE ************************/
void	boucle_str(t_token **head);
void	clear_actual_command(t_token **head);
/*************************** TEMPORAIRE ************************/

#endif