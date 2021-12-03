/***************************************
* Project - Compiler for IFJ21
*
* @brief Implementation of symtable (BST)
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file symtable.h
**************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "my_string.h"
#include "scanner.h"

typedef struct data_func {
    bool declared;
    bool defined;
    int num_params;
    int num_return;
    char *pole_params; //pole[int, string, int]
    char *pole_return; 
    struct symtable *local_symtable;
} *tData_func;

typedef struct data_var {
    String data_type;
    Token_value value;
} *tData_var;


typedef struct node {
    String id; //Name of the function/variable, key
    struct data_func *tdataf; //data about the function
    struct data_var *tdatav; //data about the function
    struct node *left;
    struct node *right;
} *tNode;

typedef struct symtable {
    tNode *root;
} tSymtable;

void init_symtable(tSymtable *symtable);
tNode insert_var(tNode *root, Token token);
tNode insert_func(tNode *root, Token token);




#endif //SYMTABLE_H