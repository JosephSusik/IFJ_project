CC = gcc
CFLAGS = -std=c99 -Wall -Wextra 

FILES = scanner.o compilerIFJ21.o parser.o my_string.o

all: build

build: $(FILES) error.h
	$(CC) $(CFLAGS) $(FILES) -o IFJ21

compilerIFJ20.o: compilerIFJ21.c parser.o
	$(CC) $(CFLAGS) -c compilerIFJ21.c

parser.o: parser.c parser.h scanner.o
	$(CC) $(CFLAGS) -c parser.c

scanner.o: scanner.c scanner.h my_string.o
	$(CC) $(CFLAGS) -c scanner.c

my_string.o: my_string.c my_string.h
	$(CC) $(CFLAGS) -c my_string.c