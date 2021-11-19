/***************************************
* Project - Compiler for IFJ21
*
* @brief Scan input file and generate tokens
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
*
* @file parser.h
**************************************/

#ifndef PARSER_H
#define PARSER_H



#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

typedef struct parser {
    Token token;
} * Parser;

Parser initParser();

int parse();


#endif //PARSER_H
