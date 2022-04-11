#native compiler
CC=gcc

#compilation flags
CFLAGS=-I -g -Wall -Werror

all: server-test

default: server-test

server-test: server-test.o
			$(CC) $(CFLAGS) server-test.c -o server-test
			
exec=server-test

.PHONY: clean
clean:
		rm -f $(exec) *.o