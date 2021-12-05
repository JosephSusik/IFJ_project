/***************************************
* Project - Compiler for IFJ21
*
* @brief Symtable for IFJ21
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
* 
*
* @file symtable.h
**************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "my_string.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    var,
    func
} node_type;

typedef struct function_data_struct {
    bool declared;
    bool defined;
    int num_params;
    int num_return;
    char *pole_params;
    char *pole_returns;
} function_data;

typedef struct node {
    String *key; //key, probably id token->tvalue->string
    node_type type; // is it var (0) or function (1)
    function_data *func_data; // data about function
    struct node *lptr; //left pointer
    struct node *rptr; //right pointer
} *nodeptr;

typedef struct symtable_struct {
    nodeptr root;
} symtable;


void tree_init(nodeptr *root);
nodeptr tree_search(nodeptr root, String *k);
void tree_insert(nodeptr *root, String *k, node_type type, function_data *data);
void ReplaceByRightmost (nodeptr ptr_replaced, nodeptr *root);
void tree_delete(nodeptr *root, String *k);
void tree_dispose(nodeptr *root);

void symtable_init(symtable *sroot);
nodeptr symtable_search(symtable *sroot, String *k);
void symtable_insert(symtable *sroot, String *k, node_type type); //need to pass more params, prob
void symtable_delete(symtable *sroot, String *k);
void symtable_dispose(symtable *sroot);

//TO-DO: add pointer to local tree/symtable, more params symtable_insert


#endif //SYMTABLE_H

