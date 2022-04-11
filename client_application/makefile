#native compiler
CC=gcc

#compilation flags
CFLAGS=-I -g -Wall -Werror

all: client

default: client

client: client.o
			$(CC) $(CFLAGS) client.c -o client
			
exec=client

.PHONY: clean
clean:
		rm -f $(exec) *.o