SRCS = \
	main.c

OBJS = $(SRCS:.c=.o)

NAME = minishell
HEADER = minishell.h

LIBFT_DIR = ./Libft/libft
LIBFT = ${LIBFT_DIR}/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -g3

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)


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

