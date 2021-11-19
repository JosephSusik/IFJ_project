CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

FILES = main.o my_string.o scanner.o parser.o

all: build

build: $(FILES) error.h
	$(CC) $(CFLAGS) $(FILES) -o project

main.o: main.c scanner.h
	$(CC) $(CFLAGS) -c main.c

my_string.o: my_string.c my_string.h
	$(CC) $(CFLAGS) -c my_string.c

scanner.o: scanner.c scanner.h my_string.h error.h
	$(CC) $(CFLAGS) -c scanner.c

parser.o: parser.c parser.h scanner.h error.h
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f *.o project
