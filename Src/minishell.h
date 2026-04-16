#ifndef  MINISHELL_H
# define MINISHELL_H

#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdbool.h>

#include "../Libft/libft/libft.h"

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
	t_type	type;
	char	*value;
	struct s_token	*next;
}	t_token;

typedef	struct s_parse
{
	int	pos_first_quote;
	int	pos_sec_quote;
}	t_parse;

volatile sig_atomic_t   signal_received; // utilisation dune variable volatile car cest un e variable dont la valeur va etre modifier en dehors du programme --> ASSURE UNE PORTABILITE GENERALE


// pq sigaction ?plus complexe que signal, early version diidn't automatically restart systeme calls, signal is set back to its default behavior each time so we need to restablish the handler every time after the signal had occured 
// sigaction(SIGHUP, signal type
//          &newact, structure sigaction: sa_handler -> point to handler or SIG_IGN (ignore)/SIG_DFL(default), sa_mask -> set of signals to be blocked during executiopn of handler, sa_flags to set to control the call with more details
//          &oldact); where to return the old handler info ( NUL if not interested)
//

#endif