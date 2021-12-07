/***************************************
* Project - Compiler for IFJ21
*
* @brief Implementation of stack
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file stack.c
**************************************/

#include "stack.h"

void stack_init(stack *Stack) {
    Stack->top = NULL;
}

void stack_dispose(stack *Stack) {
    while(Stack->top != NULL) {
        stack_pop(Stack);
    }
}

stackptr *stack_top(stack *Stack) {
    stackptr *tmp = Stack->top;
    return tmp;
}

int stack_push(stack *Stack, String d) {
    stackptr *tmp = malloc(sizeof(stackptr));

    if (tmp == NULL) {
        return INTERNAL_ERR;
    }

    string_init(&tmp->data);
    string_copy(&tmp->data, &d);
    tmp->next = Stack->top;
    Stack->top = tmp;
//    string_print(&Stack->top->data);
//    printf("\n");
    return OK;
}

int stack_pop(stack *Stack) {
    stackptr *tmp = NULL;

    if (Stack->top == NULL) {
        return 1;
    }

    tmp = Stack->top;
    Stack->top = tmp->next;
    free(tmp);

    return 0;
}
