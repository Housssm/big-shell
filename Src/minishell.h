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
# include <limits.h>
# include <unistd.h>
# include <string.h>

# include <unistd.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "../Libft/libft/libft.h"

typedef enum e_type
{
	PIPE,
	WORD,
	INREDIR,
	OUTREDIR,
	HEREDOC,
	APPOUTREDIR,
	DOUBLE,
	SIMPLE,
	ESPACE,
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
	NO_PIPE_PARS,
}	t_pars_type;

// typedef struct s_lst_fd
// {
// 	char			*file;
// 	int				fd;
// 	struct s_lst_fd	*previous;
// 	struct s_lst_fd	*next;
// }	t_lst_fd;

typedef struct s_lst_fd
{
	char			*file;
	int				fd;
	int				type;        // INREDIR_PARS, OUTREDIR_PARS, etc.
	int				target_fd;   // STDIN_FILENO ou STDOUT_FILENO
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
	int				error;
}	t_tree;

typedef struct s_parse
{
	int	pos_first_quote;
	int	pos_sec_quote;
}	t_parse;

typedef struct s_env
{
    char    *key;   /* PATH */
    char    *value;   /* /usr/bin:/bin */
    struct s_env *next;
}   t_env;


// Lexer
char	*get_line(void);
void	handler(int sigtype);
int		new_token(char c, int *flag);
int		handle_quote(t_token **cmd, char *line, int *i, int *in_quote);
int		process_token(t_token **cmd, char *line, int *i, int *in_quote);
int		parse_line(t_token **cmd, char *line);
t_token	*ft_add_last(t_token *head);
void	define_cmd(t_token *cmd, t_type opcode);
void	what_is_it(t_token *cmd, char *str);
t_token	*create_token_from_line(char *line, int beg, int end);
int		add_node(t_token **head, char *line, int beg, int end);
// int		add_node(t_token **head, char *line, int beg, int end);
int		is_blank(char *str);
int		is_blank(char *str);
int		is_space(char c);
int		join_two_token(t_token *t1, t_token *t2, t_token *t3);
int		check_post_redir(t_token *head);
int		join_word_to_dbl_quote(t_token **head);
void	clear_actual_command(t_token **head);

// void	lexer(t_tree *tree, char *line);

/*************************** TEMPORAIRE ************************/
void	boucle_str(t_token **head);
void	clear_actual_command(t_token **head);
void	print_tree(t_tree *tree);

/*************************** TEMPORAIRE ************************/

/* BUILTINS */
int update_env(t_env **env, char *key, char *value);
t_env   *init_env(char **envp);
int    ft_cd(char **av, t_env **env);
int    ft_echo(char **av);
int    ft_export(char **av, t_env **env);
int	   ft_pwd(void);
int    ft_unset(char **av, t_env **env);
int    ft_env(t_env *env);
int    ft_exit(char **av, t_env *env);

//Parsing
void	free_tree(t_tree *tree);
void	what_is_ptype(t_tree *tree, t_token *cmd);
int		value_from_list_to_tree(t_tree *branch, t_token **cmd, size_t count);
bool	search_pipe(t_token **cmd, size_t *count);
t_token	*new_head_actualisation(t_token **head, size_t count);
t_tree	*left_branch(t_tree *tree, t_token **cmd, size_t count);
t_tree	*new_pipe(t_tree *tree, t_token **cmd, size_t *count);
t_tree	*no_pipe_tree(t_tree *tree, t_token **cmd, size_t *count);
// t_tree	*parser(t_token **cmd);
t_tree	*parser(t_token **cmd);
// int		lexer(t_tree **tree, char *line);
int lexer(t_tree **tree, char *line, t_env **env, int *last_status);



//Pipex
typedef struct s_data
{
	size_t	ac;
	pid_t	**pip;
	int		*pid;
	char	**args;
	char	**cmd;
	char	**env;
	char	*path;
	int		in_fd;
	int		out_fd;
	int		is_heredoc;

}t_data;

void	free_struct(t_data *data);
char	*ft_strjoin_three(char *s1, char *s2, char *s3);
int		struct_attribution(int ac, char **av, char**env, t_data *data);
int		extract_path(t_data *data, char **full_path, char *av);
int		check_existing_path(t_data *data, char *av);
void	free_all_struct(t_data *data);
void	closing_pipes(t_data *data, size_t n);
int		check_errors(int ac, char **av, char **env, t_data *data);
int		check_path(t_data *data, char *av);
int		check_errors(int ac, char **av, char **env, t_data *data);
int		is_heredoc(int ac, char **av, char **env, t_data *data);
int		cmd_excecution(t_data *data, char *av, size_t n);
int		pi_opening(t_data *data);
void	wait_for_pid(t_data *data);
void	run_execution(char **av, t_data *data, int i);
int		check_error_bonus(int ac, char **av, char **env, t_data *data);
int		check_is_heredoc(int ac, char **av, t_data *data);
int		check_env(char **env);

//Executor
int	open_heredoc(char *delimiter);
int	open_input_file(char *file);
int	open_output_file(char *file);
int	open_append_file(char *file);
int	dup_redir_fd(int fd, int std_fd);
int	apply_redirections(t_lst_fd *fds);
int	run_builtin(t_tree *node, t_env **env, int *last_status);
void	exec_child(t_tree *node, t_env **env, char *path);
int	execute_command(t_tree *node, t_env **env, int *last_status);
void	pipe_child_left(t_tree *node, int pipe_fd[2],t_env **env, int *status);
void	pipe_child_right(t_tree *node, int pipe_fd[2],t_env **env, int *status);
int	exec_pipe(t_tree *node, t_env **env, int *last_status);
int	shell_execute(t_tree *tree, t_env **env, int *last_status);
t_env	*find_env_var(t_env *env, char *name);
void    free_env(t_env *head);




// int		shell_execute(t_tree *tree, t_env **env, int *last_status);
int		is_builtin(char *cmd);
// int		run_builtin(t_tree *node, t_env **env, int *last_status);
char	**env_to_tab(t_env *env);
char	*get_cmd_path(char *cmd, t_env *env);



#endif