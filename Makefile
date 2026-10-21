NAME = rogue

CC = gcc
STD = --std=c99
POSIX = -D_POSIX_C_SOURCE=199309L
CFLAGS = -Wall -lncurses -I$(IDIR)

SRCDIR = ./src/
IDIR = ./include/
WEBDIR = ./web/

SOURCES = $(SRCDIR)*.c

.PHONY: all run test clean web

all: $(NAME)

$(NAME): clean
	$(CC) $(STD) $(POSIX) $(SOURCES) $(CFLAGS) -o $@

run:
	./$(NAME)

test:
	valgrind ./$(NAME)

clean:
	if [ -e $(NAME) ]; then rm $(NAME); fi

web:
	mkdir -p $(WEBDIR)
	emcc $(STD) $(POSIX) $(SOURCES) -I$(IDIR) -sASYNCIFY=1 -sASYNCIFY_IMPORTS=js_getch_async -sASYNCIFY_STACK_SIZE=65536 -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_RUNTIME_METHODS=['UTF8ToString','ccall','cwrap'] -o $(WEBDIR)rogue.js
