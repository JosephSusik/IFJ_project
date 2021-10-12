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
#include "scanner.h"

typedef struct {
  Token *array;
  size_t used;
  size_t size;
} TokenArray;

void parser(char *p_text, int numbytes);