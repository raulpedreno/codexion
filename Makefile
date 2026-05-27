NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC =	src/main.c \
		src/parse.c \
		src/init.c \
		src/cleanup.c \
		src/log.c \
		src/utils.c \
		src/time.c \
		src/routine.c \
		src/threads.c \
		src/monitor.c \
		src/dongles.c \
		src/scheduler.c

OBJ = $(SRC:.c=.o)

INCLUDE = -I include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c include/codexion.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re