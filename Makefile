
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g -I./ -lm
BIN=example

all:
	$(CC) -o $(BIN) main.c cterm.c $(CFLAGS)

clean:
	rm -f $(BIN)
