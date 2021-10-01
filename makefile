CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

FILES = scanner.o compilerIFJ21.o parser.o

all: build

build: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o IFJ21

compilerIFJ20.o: compilerIFJ21.c parser.o
	$(CC) $(CFLAGS) -c compilerIFJ21.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c