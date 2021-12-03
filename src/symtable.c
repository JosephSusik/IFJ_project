/***************************************
* Project - Compiler for IFJ21
*
* @brief Implementation of symtable (BST)
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file symtable.c
**************************************/

#include "symtable.h"

void init_symtable(tSymtable *symtable) {
    symtable->root = NULL;
}