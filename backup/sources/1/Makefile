NAME = rogue

CC = gcc
STD = --std=c99
POSIX = -D_POSIX_C_SOURCE=199309L
CFLAGS = -lncurses -I$(IDIR)

SRCDIR = ./src/
IDIR = ./include/

SOURCES = $(SRCDIR)*.c

all: $(NAME)

$(NAME): clean
	$(CC) $(STD) $(POSIX) $(SOURCES) $(CFLAGS) -o $@
run:
	./$(NAME)

test:
	valgrind ./$(NAME)

clean:
	if [ -e $(NAME) ]; then rm $(NAME); fi

