#include "../minishell.h"

/************************************* */
int	open_heredoc(char *delimiter)
{
	int		fd;
	char	*line;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (open(".heredoc_tmp", O_RDONLY));
}
/********************* */
int	open_input_file(char *file)
{
	return (open(file, O_RDONLY));
}

int	open_output_file(char *file)
{
	return (open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
}

int	open_append_file(char *file)
{
	return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

int	open_redir(t_lst_fd *fds, int std_fd)
{
    (void)std_fd;
	if (fds->type == INREDIR_PARS)
		return (open_input_file(fds->file));
	if (fds->type == OUTREDIR_PARS)
		return (open_output_file(fds->file));
	if (fds->type == APPOUTREDIR_PARS)
		return (open_append_file(fds->file));
	if (fds->type == HEREDOC_PARS)
		return (open_heredoc(fds->file));
	return (-1);
}
/******************************************* */
int	dup_redir_fd(int fd, int std_fd)
{
	if (fd < 0)
		return (1);
	if (dup2(fd, std_fd) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_lst_fd *fds)
{
	t_lst_fd	*current;
	int			fd;

	current = fds;
	while (current)
	{
		fd = open_redir(current, current->target_fd);
		if (fd == -1)
		{
			perror(current->file);
			return (1);
		}
		if (dup_redir_fd(fd, current->target_fd))
			return (1);
		current = current->next;
	}
	return (0);
}
/************************** */
// int	run_builtin(t_tree *node, t_env **env, int *last_status)
// {
//     (void)last_status;
// 	if (ft_strcmp(node->av[0], "echo") == 0)
// 		return (ft_echo(node->av));
// 	if (ft_strcmp(node->av[0], "cd") == 0)
// 		return (ft_cd(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "pwd") == 0)
// 		return (ft_pwd());
// 	if (ft_strcmp(node->av[0], "export") == 0)
// 		return (ft_export(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "unset") == 0)
// 		return (ft_unset(node->av + 1, env));
// 	if (ft_strcmp(node->av[0], "env") == 0)
// 		return (ft_env(*env));
// 	if (ft_strcmp(node->av[0], "exit") == 0)
// 		return (ft_exit(node->av));
// 	return (127);
// }
/********************************** */
void	exec_child(t_tree *node, t_env **env, char *path)
{
	char	**envp;

	if (apply_redirections(node->fds))
		exit(1);
	if (is_builtin(node->av[0]))
		exit(run_builtin(node, env, NULL));
	envp = env_to_tab(*env);
	execve(path, node->av, envp);
	free_split(envp);
	perror("execve");
	exit(126);
}

int	execute_command(t_tree *node, t_env **env, int *last_status)
{
	char	*path;
	pid_t	pid;

	if (is_builtin(node->av[0]) && !node->fds)
		return (run_builtin(node, env, last_status));

	path = get_cmd_path(node->av[0], *env);
    printf("333333\n\n\n\n");
    printf("le path %s\n\n\n", path);
	if (!path && !is_builtin(node->av[0]))
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(node->av[0], 2);
		ft_putstr_fd("\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_child(node, env, path);
	free(path);
	waitpid(pid, last_status, 0);
	return (WEXITSTATUS(*last_status));
}
/**************************************** */
void	pipe_child_left(t_tree *node, int pipe_fd[2], t_env **env, int *status)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exit(shell_execute(node->left, env, status));
}

void	pipe_child_right(t_tree *node, int pipe_fd[2], t_env **env, int *status)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exit(shell_execute(node->right, env, status));
}

int	exec_pipe(t_tree *node, t_env **env, int *last_status)
{
    // printf("2222222\n\n\n\n");

	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (perror("fork"), 1);
	if (left_pid == 0)
		pipe_child_left(node, pipe_fd, env, last_status);
	right_pid = fork();
	if (right_pid == -1)
		return (perror("fork"), 1);
	if (right_pid == 0)
		pipe_child_right(node, pipe_fd, env, last_status);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, last_status, 0);
	return (WEXITSTATUS(*last_status));
}
/***************************************** */
int	shell_execute(t_tree *tree, t_env **env, int *last_status)
{
    printf("11111111\n\n\n\n");

	if (!tree)
		return (0);
	if (tree->parse_type == PIPE_PARS)
		return (exec_pipe(tree, env, last_status));
	return (execute_command(tree, env, last_status));
}
























// je souhaite reproduire le comportement de bash de maniere sommaire dans le cadre du projet minishel de  lecole 42
// Jai deja realise la partie de la tokenisation et du lexing. Je souhaite desormais implementer la partie excecution de ce projet. 
// Jai par le passe realise un projet independant qui consiste a reproduire le comportement des pipes et du heredoc. independnt dou des conditions de different notamment mais globalement la logique est similaire dans un certains sens. 
// dautre part jai aussi realiser les fonctoins qui ne peuvent pas etre utiliser par excecve comme cd cat.... tu trouvera mon code pour le projet pipex ci dessous et le prototypage des fonctions "builtins" plus bas, elles ont le meme comportement que les fonctions dorigine
// jattends de toi que tu remodele mon projet pipex pour le rendre utilisable par ma fonction lexer ( fonction ou est realise la tokenisation et le parsing) et que tu implement aussi lutilisation de mes fonctions si besoin
// les fonctions que tu me donnera ne devront pas depasser 25 lignes et prends plus de 4 parametre par fonction.
// je tai envoye toutes les fonctions que je juge necessaire mais si tu en as besoin tu trouvera dautres fichiers avec tout mon code 


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
// 	clear_actual_command(&cmd);
// /*     inserer fonction ici*/
//     free_tree(*tree);
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


// main de la fonction que je souhaite fusionner

// int	main(int ac, char **av, char **env)
// {
// 	t_data		data;
// 	size_t		i;

// 	data.is_heredoc = 0;
// 	if (check_is_heredoc(ac, av, &data))
// 		return (1);
// 	if (check_error_bonus(ac, av, env, &data))
// 		return (1);
// 	if (struct_attribution(ac, av, env, &data))
// 		return (1);
// 	if (pi_opening(&data))
// 		return (1);
// 	i = 0;
// 	while (i < data.ac)
// 	{
// 		run_execution(av, &data, i);
// 		i++;
// 	}
// 	closing_pipes(&data, -1);
// 	close(data.in_fd);
// 	close(data.out_fd);
// 	wait_for_pid(&data);
// 	free_all_struct(&data);
// 	return (0);
// }

// int	reading_and_writing(char **av)
// {
// 	int		fd_write;
// 	char	*str;
// 	int		len;

// 	len = ft_strlen(av[2]);
// 	fd_write = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd_write == -1)
// 		return (close(fd_write), 1);
// 	while (1)
// 	{
// 		str = get_next_line(0);
// 		if (!str)
// 			return (close(fd_write), 1);
// 		if (strncmp(str, av[2], (len)) == 0 && str[len] == '\n')
// 		{
// 			ft_putstr_fd("\0", fd_write);
// 			(free(str));
// 			break ;
// 		}
// 		else
// 			ft_putstr_fd(str, fd_write);
// 		free(str);
// 	}
// 	close(fd_write);
// 	return (0);
// }

// int	check_is_heredoc(int ac, char **av, t_data *data)
// {
// 	if (ft_strncmp(av[1], "here_doc", 9) != 0)
// 		return (data->is_heredoc = 0, 0);
// 	if (ac != 6)
// 		return (perror("Invalid argument\n"), 1);
// 	data->is_heredoc = 1;
// 	if (reading_and_writing(av))
// 		return (1);
// 	return (0);
// }

// int	check_error_bonus(int ac, char **av, char **env, t_data *data)
// {	
// 	if (ac < 5)
// 		return (ft_printf("Check arguments\n"), 14);
// 	else if (check_env(env))
// 		return (perror("pipex 1"), 1);
// 	if (data->is_heredoc == 1)
// 		return (0);
// 	else if (access(av[1], F_OK) != 0)
// 		return (perror("pipex 2"), 126);
// 	else if (access(av[ac - 1], F_OK) != 0)
// 	{
// 		data->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (data->out_fd == -1)
// 			return (close(data->out_fd), free_all_struct(data),
// 				perror("pipex outfil"), 1);
// 		close(data->out_fd);
// 	}
// 	else if (access(av[1], R_OK) != 0 || access(av[ac -1], W_OK) != 0)
// 		return (perror("pipex 4"), 126);
// 	return (0);
// }


// int	extract_path(t_data *data, char **full_path, char *av)
// {
// 	char	*str;

// 	int (i) = 0;
// 	int (verif) = 1;
// 	data->cmd = ft_split(av, ' ');
// 	if (!data->cmd)
// 		return (free_all_struct(data), 1);
// 	if (data->cmd[0] == (void *)0)
// 		return (1);
// 	while (full_path[i])
// 	{	
// 		str = ft_strjoin_three(full_path[i], "/", data->cmd[0]);
// 		if (access(str, F_OK | R_OK) == 0)
// 		{
// 			data->path = ft_strdup(str);
// 			if (!data->path)
// 				return (free_all_struct(data), 1);
// 			(free(str), verif = 0);
// 			break ;
// 		}
// 		(free(str), i++);
// 	}
// 	if (verif == 1)
// 		return (ft_printf("pipex3: command not found: %s\n", data->cmd[0]), 1);
// 	return (0);
// }

// int	check_existing_path(t_data *data, char *av)
// {	
// 	if (ft_strchr(av, '/'))
// 	{
// 		data->cmd = ft_split(av, ' ');
// 		if (!data->cmd)
// 			return (1);
// 		if (access(data->cmd[0], F_OK | R_OK) == 0)
// 		{
// 			data->path = ft_strdup(data->cmd[0]);
// 			if (!data->path)
// 				return (free_split(data->cmd), 1);
// 			return (0);
// 		}
// 		else if (access(data->cmd[0], F_OK | R_OK) != 0)
// 			return (ft_printf("pipex 22:"
// 					"no such file or directory: %s\n", av), 1);
// 	}
// 	return (0);
// }

// int	check_path(t_data *data, char *av)
// {
// 	int		i;
// 	char	**full_path;

// 	i = 0;
// 	if (check_existing_path(data, av))
// 		return (1);
// 	if (data->path != NULL)
// 		return (0);
// 	full_path = NULL;
// 	while (data->env[i])
// 	{
// 		if (ft_strncmp(data->env[i], "PATH=", 5) == 0)
// 		{
// 			full_path = ft_split(data->env[i] + 5, ':');
// 			if (!full_path)
// 				return (free_split(full_path), 1);
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (extract_path(data, full_path, av))
// 		return (free_split(full_path), 1);
// 	free_split(full_path);
// 	return (0);
// }

// void	wait_for_pid(t_data *data)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < data->ac)
// 	{
// 		if (data->pid[i] > 0)
// 			waitpid(data->pid[i], NULL, 0);
// 		i++;
// 	}
// }


// int	check_env(char **env)
// {
// 	size_t	i;
// 	int		verif;

// 	i = 0;
// 	verif = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], "PATH=", 5) == 0)
// 			verif += 1;
// 		i++;
// 	}
// 	if (verif == 0)
// 		return (1);
// 	return (0);
// }

// int	check_errors(int ac, char **av, char **env, t_data *data)
// {	
// 	if (ac != 5)
// 		return (ft_printf("Check arguments\n"), 14);
// 	else if (check_env(env))
// 		return (perror("pipex 1"), 1);
// 	if (data->is_heredoc == 1)
// 		return (0);
// 	else if (access(av[1], F_OK) != 0)
// 		return (perror("pipex 2"), 126);
// 	else if (access(av[ac - 1], F_OK) != 0)
// 	{
// 		data->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (data->out_fd == -1)
// 			return (close(data->out_fd), free_all_struct(data),
// 				perror("pipex outfil"), 1);
// 		close(data->out_fd);
// 	}
// 	else if (access(av[1], R_OK) != 0 || access(av[ac -1], W_OK) != 0)
// 		return (perror("pipex 4"), 126);
// 	return (0);
// }

// int	pi_intialisation(t_data *data)
// {
// 	size_t	i;

// 	i = 0;
// 	data->pip = malloc(sizeof(int *) * (data->ac - 1));
// 	data->pid = ft_calloc(data->ac, sizeof(pid_t));
// 	if (!data->pip || !data->pid)
// 		return (ft_putstr_fd("Error malloc array\n", 2),
// 			free_all_struct(data), 1);
// 	while (i < data->ac - 1)
// 	{
// 		data->pip[i] = malloc(sizeof(int) * 2);
// 		if (!data->pip[i])
// 			return (free_all_struct(data), 1);
// 		i++;
// 	}
// 	return (0);
// }

// int	fd_opening(int ac, char **av, t_data *data)
// {
// 	if (data->is_heredoc == 1)
// 	{
// 		data->in_fd = open(".heredoc_tmp", O_RDONLY);
// 		data->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	}
// 	else
// 	{
// 		data->in_fd = open(av[1], O_RDONLY);
// 		data->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	}
// 	if (data->out_fd == -1 || data->in_fd == -1)
// 		return (close(data->in_fd), free_all_struct(data),
// 			perror("pipex outfile"), 1);
// 	return (0);
// }

// int	struct_attribution(int ac, char **av, char**env, t_data *data)
// {	
// 	data->ac = ac - 3;
// 	if (data->is_heredoc == 1)
// 		data->ac = 2;
// 	data->env = env;
// 	if (pi_intialisation(data))
// 		return (1);
// 	if (!data->env)
// 		return (ft_putstr_fd("Error copy env\n", 2), 1);
// 	data->args = av;
// 	if (!data->args)
// 		return (ft_putstr_fd("Error copy args\n", 2), 1);
// 	data->cmd = NULL;
// 	data->path = NULL;
// 	if (fd_opening(ac, av, data))
// 		return (1);
// 	return (0);
// }


// void	closing_pipes(t_data *data, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	if (n == (size_t)(-1))
// 	{
// 		while (i < data->ac - 1)
// 		{
// 			if (data->pip[i][0] >= 0)
// 				close(data->pip[i][0]);
// 			if (data->pip[i][1] >= 0)
// 				close(data->pip[i][1]);
// 			i++;
// 		}
// 		return ;
// 	}
// 	while (i < data->ac - 1)
// 	{
// 		if (i + 1 != n)
// 			close(data->pip[i][0]);
// 		if (i != n)
// 			close (data->pip[i][1]);
// 		i++;
// 	}
// }

// void	free_struct(t_data *data)
// {
// 	if (data->path)
// 	{
// 		free(data->path);
// 		data->path = NULL;
// 	}
// 	if (data->cmd)
// 	{
// 		free_split(data->cmd);
// 		data->cmd = NULL;
// 	}
// }

// void	free_tab(t_data *data, int **array)
// {
// 	size_t	i;

// 	i = 0;
// 	if (array)
// 	{
// 		while (i < data->ac - 1)
// 		{
// 			free(array[i]);
// 			i++;
// 		}
// 		free(array);
// 	}
// }

// void	free_all_struct(t_data *data)
// {
// 	free_struct(data);
// 	if (data->pid)
// 	{
// 		free(data->pid);
// 		data->pid = NULL;
// 	}
// 	free_tab(data, data->pip);
// 	close(data->in_fd);
// 	close(data->out_fd);
// 	if (data->is_heredoc == 1)
// 		unlink(".heredoc_tmp");
// }

// char	*ft_strjoin_three(char *s1, char *s2, char *s3)
// {
// 	char	*result;
// 	int		i;
// 	int		j;
// 	int		size;

// 	size = ft_strlen((char *)s1)
// 		+ ft_strlen((char *)s2) + ft_strlen((char *)s3);
// 	result = malloc(sizeof(char) * (size + 1));
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (s1[i])
// 	{
// 		result[i] = s1[i];
// 		i++;
// 	}
// 	while (s2[j])
// 		result[i++] = s2[j++];
// 	j = 0;
// 	while (s3[j])
// 		result[i++] = s3[j++];
// 	result[i] = '\0';
// 	return (result);
// }

// voici le prototype des fonctions du meme nom

// int    ft_cd(char **av, t_env **env);
// int    ft_echo(char **av);
// int    ft_export(char **av, t_env **env);
// int	   ft_pwd(void);
// int    ft_unset(char **av, t_env **env);
// int    ft_env(t_env *env);
// int    ft_exit(char **av);








































// // #include "../minishell.h"

// // static int	exec_pipe(t_tree *node, t_env **env, int *last_status);
// // static int	exec_cmd(t_tree *node, t_env **env, int *last_status);

// // int	shell_execute(t_tree *tree, t_env **env, int *last_status)
// // {
// //     if (!tree)
// //         return (0);
// //     if (tree->parse_type == PIPE_PARS)
// //         return (exec_pipe(tree, env, last_status));
// //     else
// //         return (exec_cmd(tree, env, last_status));
// // }

// // static int	exec_pipe(t_tree *node, t_env **env, int *last_status)
// // {
// //     int		pipe_fd[2];
// //     pid_t	left_pid;
// //     pid_t	right_pid;

// //     if (pipe(pipe_fd) == -1)
// //         return (perror("pipe"), 1);
// //     left_pid = fork();
// //     if (left_pid == -1)
// //         return (perror("fork"), 1);
// //     if (left_pid == 0)
// //     {
// //         close(pipe_fd[0]);
// //         dup2(pipe_fd[1], STDOUT_FILENO);
// //         close(pipe_fd[1]);
// //         exit(shell_execute(node->left, env, last_status));
// //     }
// //     right_pid = fork();
// //     if (right_pid == -1)
// //         return (perror("fork"), 1);
// //     if (right_pid == 0)
// //     {
// //         close(pipe_fd[1]);
// //         dup2(pipe_fd[0], STDIN_FILENO);
// //         close(pipe_fd[0]);
// //         exit(shell_execute(node->right, env, last_status));
// //     }
// //     close(pipe_fd[0]);
// //     close(pipe_fd[1]);
// //     waitpid(left_pid, NULL, 0);
// //     waitpid(right_pid, last_status, 0);
// //     return (WEXITSTATUS(*last_status));
// // }

// // static int	exec_cmd(t_tree *node, t_env **env, int *last_status)
// // {
// //     char	*path;
// //     char	**envp;
// //     pid_t	pid;

// //     if (is_builtin(node->av[0]))
// //         return (run_builtin(node, env, last_status));
// //     path = get_cmd_path(node->av[0], *env);
// //     if (!path)
// //     {
// //         ft_putstr_fd("minishell: command not found: ", 2);
// //         ft_putstr_fd(node->av[0], 2);
// //         ft_putstr_fd("\n", 2);
// //         return (127);
// //     }
// //     pid = fork();
// //     if (pid == -1)
// //         return (perror("fork"), 1);
// //     if (pid == 0)
// //     {
// //         envp = env_to_tab(*env);
// //         execve(path, node->av, envp);
// //         free(path);
// //         free_split(envp);
// //         perror("execve");
// //         exit(126);
// //     }
// //     waitpid(pid, last_status, 0);
// //     free(path);
// //     return (WEXITSTATUS(*last_status));
// // }
