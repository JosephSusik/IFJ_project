/***************************************
* Project - Compiler for IFJ21
*
* @brief Library for easier string handeling
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file my_string.h
**************************************/

#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <string.h>


typedef struct {
    char* str;
    int length;
    int alloc_size;
} String;

int string_init(String *s);
void string_free(String *s);
void string_clear(String *s);
int string_add_char(String *s, char c);
int string_cmp(String *s, const char *keyword_string);
int string_string_cmp(String *s, String *s2);
void string_print(String *s);
int string_copy(String *s1, String *s2);
int string_strtok(String *s1, const char *d, String *s2);



#endif // MY_STRING_H
