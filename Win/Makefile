NAME = rogue

CC1 = i686-w64-mingw32-gcc
CC2 = x86_64-w64-mingw32-gcc
STD = --std=c99
POSIX = -D_POSIX_C_SOURCE=199309L
CFLAGS = -Wall -I/usr/i686-w64-mingw32/include/ncursesw -I$(IDIR)

SRCDIR = ../src/
IDIR = ../include/

SOURCES = $(SRCDIR)*.c

all: $(NAME)

$(NAME): clean
	$(CC1) $(STD) $(POSIX) $(SOURCES) $(CFLAGS) -o $@-32.exe
	$(CC2) $(STD) $(POSIX) $(SOURCES) $(CFLAGS) -o $@-64.exe

clean:
	if [ -e $(NAME)-32.exe ]; then rm $(NAME)-32.exe; fi
	if [ -e $(NAME)-64.exe ]; then rm $(NAME)-64.exe; fi
