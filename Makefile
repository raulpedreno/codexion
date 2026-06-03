NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC =	coders/main.c \
		coders/parse.c \
		coders/init.c \
		coders/cleanup.c \
		coders/log.c \
		coders/utils.c \
		coders/time.c \
		coders/routine.c \
		coders/threads.c \
		coders/monitor.c \
		coders/dongles.c \
		coders/dongles_utils.c \
		coders/dongles_take.c \
		coders/dongles_signal.c \
		coders/scheduler.c

OBJ = $(SRC:.c=.o)

INCLUDE = -I coders

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c coders/codexion.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re