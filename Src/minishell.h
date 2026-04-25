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

extern volatile sig_atomic_t	g_signal_received; // var dt valeur pt etre mod en dehors progr -> assure portabilite general

// Lexer
char	*get_line(void);
void	handler(int sigtype);



#endif