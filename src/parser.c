/***************************************
* Project - Compiler for IFJ21
*
* @brief Scan input file and generate tokens
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
*
* @file parser.c
**************************************/

#include "parser.h"

Parser initParser() {
    Parser parser = malloc(sizeof(struct parser));
    return parser;
}

int parse() {
    Parser parser = initParser();

    if(getNextToken(&parser->token) == 0) {
        printf("TOKEN \n");
        if(parser->token.tuniontype == 4) {
            printf("NO TOKEN \n");
        }
    }
    return 0;
}