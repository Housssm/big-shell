SRCS =    ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
	ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c \
	ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c \
	ft_strdup.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c \
	ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c \
	ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
	ft_split.c ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_substr.c ft_striteri.c \
	ft_strcmp.c ft_strndup.c 

OBJS = ${SRCS:.c=.o}

NAME = libft.a

PRINTF_DIR = ../printf
PRINTF = ${PRINTF_DIR}/libftprintf.a

GNL_DIR = ../GNL
GNL = ${GNL_DIR}/get_next_line.a

CC = cc

CFLAGS = -Wall -Werror -Wextra -g3

all:     ${NAME}

${NAME}: ${PRINTF} ${GNL} ${OBJS}
	ar -rcs ${NAME} ${OBJS} ${PRINTF_DIR}/*.o ${GNL_DIR}/*.o

${PRINTF}:
	make -C ${PRINTF_DIR}

${GNL}:
	make -C ${GNL_DIR}

%.o: %.c libft.h
	${CC} ${CFLAGS} -I${PRINTF_DIR} -I${GNL_DIR} -c $< -o $@

clean:
	rm -f ${OBJS}
	make -C ${PRINTF_DIR} clean
	make -C ${GNL_DIR} clean

fclean:    clean
	rm -f ${NAME}
	rm -f a.out
	make -C ${PRINTF_DIR} fclean
	make -C ${GNL_DIR} fclean

re:    fclean all

.SILENT :