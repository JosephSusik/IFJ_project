CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

FILES = main.o my_string.o scanner.o parser.o code_generator.o symtable.o stack.o

all: build

build: $(FILES) error.h
	$(CC) $(CFLAGS) $(FILES) -o project

main.o: main.c scanner.h parser.h
	$(CC) $(CFLAGS) -c main.c

my_string.o: my_string.c my_string.h
	$(CC) $(CFLAGS) -c my_string.c

scanner.o: scanner.c scanner.h my_string.h error.h
	$(CC) $(CFLAGS) -c scanner.c

parser.o: parser.c parser.h scanner.h error.h code_generator.h
	$(CC) $(CFLAGS) -c parser.c

code_generator.o: code_generator.c
	$(CC) $(CFLAGS) -c code_generator.c

symtable.o: symtable.c symtable.h my_string.h
	$(CC) $(CFLAGS) -c symtable.c

stack.o: stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm -f *.o project
