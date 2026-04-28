SRCS = \
	Src/Lexer/readline.c\
	Src/Lexer/line_to_token.c\
	Src/Lexer/node_creation.c\
	Src/main.c

OBJS = $(SRCS:.c=.o)

NAME = minishell
HEADER = Src/minishell.h

LIBFT_DIR = ./Libft/libft
LIBFT = ${LIBFT_DIR}/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
READLINE_FLAGS = -lreadline -lncurses

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)


$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re

