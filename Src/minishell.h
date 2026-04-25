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
	WORD,
	PIPE,
	INREDIR,
	OUTREDIR,
	HEREDOC,
	APPOUTREDIR,
	MAIN,
	PROCESS,
	CHILD,
	WEAK_QUOTE, //dbl quote certain cas transofrme pas en literal
	STRONG_QUOTE, //simple quote de facon rigide tranfoirme en litteral
	NONE_QUOTE, //pas de quote
}	t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}	t_token;

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
void	lexer(t_token **cmd, char *line);

/*************************** TEMPORAIRE ************************/
void	boucle_str(t_token **head);
void	clear_actual_command(t_token **head);
/*************************** TEMPORAIRE ************************/



#endif