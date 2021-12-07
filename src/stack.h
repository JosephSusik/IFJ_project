/***************************************
* Project - Compiler for IFJ21
*
* @brief Implementation of stack
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file stack.h
**************************************/

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"
#include "error.h"

typedef struct stack_item{
    struct stack_item *next;
    String data;
} stackptr;

typedef struct {
    stackptr *top;
} stack;

void stack_init(stack *Stack);
void stack_dispose(stack *Stack);
stackptr *stack_top(stack *Stack);
int stack_push(stack *Stack, String d);
int stack_pop(stack *Stack);

#endif //STACK_C