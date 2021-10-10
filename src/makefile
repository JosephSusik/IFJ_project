CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror

FILES = main.o

all: build

build: $(FILES) error.h
		$(CC) $(CFLAGS) $(FILES) -o main
		
main.o: main.c
		$(CC) $(CFLAGS) -c main.c
		
clean:
	rm -f *.o main