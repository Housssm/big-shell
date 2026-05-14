SRCS = \
	Src/Lexer/readline.c\
	Src/Lexer/line_to_token.c\
	Src/Lexer/node_creation.c\
  	Src/Lexer/tokenisation_changement.c\
	Src/Parser/branch_management.c\
	Src/Parser/parser_utils.c\
	Src/builtins/builtin_cd.c\
	Src/builtins/builtin_echo.c\
	Src/builtins/builtin_export.c\
	Src/builtins/builtin_pwd.c\
	Src/builtins/builtin_unset.c \
	Src/builtins/builtin_env.c \
	Src/builtins/builtin_exit.c \
	Src/Excecution/executor.c \
	Src/Excecution/executor_utils.c \
	Src/main.c

OBJS = $(SRCS:.c=.o)

NAME = minishell
HEADER = Src/minishell.h

LIBFT_DIR = ./Libft/libft
LIBFT = ${LIBFT_DIR}/libft.a

PRINTF_DIR = ./Libft/printf
PRINTF_LIB = ${PRINTF_DIR}/libftprintf.a

GNL_DIR = ./Libft/GNL
GNL_LIB = ${GNL_DIR}/get_next_line.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
READLINE_FLAGS = -lreadline -lncurses

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(PRINTF_LIB) $(GNL_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(PRINTF_LIB) $(GNL_LIB) $(READLINE_FLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(PRINTF_LIB):
	make -C $(PRINTF_DIR)

$(GNL_LIB):
	make -C $(GNL_DIR)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean
	make -C $(PRINTF_DIR) clean
	make -C $(GNL_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean
	make -C $(PRINTF_DIR) fclean
	make -C $(GNL_DIR) fclean
.PHONY: all bonus clean fclean re