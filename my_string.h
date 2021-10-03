/***************************************
* Project - Compiler for IFJ21
* 
* @brief Scanner for IFJ21
* 
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author
* @author
* 
* @file parser.h
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* str;
    int length;
    int alloc_size;
} String;

int string_init(String *s);
int free_memory(int exit_code, String *string);
int string_add_char(String *s, char c);