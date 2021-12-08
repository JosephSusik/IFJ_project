/***************************************
* Project - Compiler for IFJ21
*
* @brief Scan input file and generate tokens
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file parser.h
**************************************/

#ifndef PARSER_H
#define PARSER_H



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "code_generator.h"
#include "symtable.h"
#include "stack.h"

typedef struct parser {
    Token token;
    int exit_code;
    int _if;
    int _else;
    int _while;
    bool end;   // if end
    bool wend;  //while end
    symtable global_symtable;
    symtable local_symtable;
    String func_id;
    int num_params;
    int num_return;
    int num_types;
    int num_args;
    stack tmp_stack;
    stack tmp_stack_2;
    stack tmp_stack_3;
    String tmp_string;
    bool label_main;
} * Parser;

int is_utype(Parser parser, int utype);
int is_kword(Parser parser, Keyword kword_2);
int is_ttype(Parser parser, int type);
int is_int_num_str(Parser parser);



Parser init_parser();
int free_parser(Parser parser);

int require(Parser parser);
int prog(Parser parser);
int params(Parser parser);
int params_2(Parser parser);
int return_type(Parser parser);
int return_type_2(Parser parser);
int type(Parser parser);
int type_2(Parser parser);
int args(Parser parser);
int args_2(Parser parser);
int body(Parser parser);

int body_if(Parser parser);
int body_else(Parser parser);
int body_while(Parser parser);



int expression_if(Parser parser);
int expression_while(Parser parser);
int expression_return(Parser parser);
int assign(Parser parser);
int id(Parser parser);







int parse();


#endif //PARSER_H
