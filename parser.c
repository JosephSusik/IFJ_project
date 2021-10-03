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
#include "error.h"
#include "parser.h"

void initTokenArray(TokenArray *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(Token));
  a->used = 0;
  a->size = initialSize;
}

void insertTokenArray(TokenArray *a, Token *element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(Token));
  }
  a->array[a->used++] = *element;
  //printf("%i\n", token->tvalue.whole_num);
}

void killTokens(TokenArray *a){
  size_t x = 0;
  while(x < a->used){
    if(a->array[x].tuniontype == 3){
      free_memory(OK, a->array[x].tvalue.string);
    }
  }
}

void freeTokenArray(TokenArray *a) {
  killTokens(a);
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void parser(char *p_text, int numbytes){
    //printf("%c\n", p_text[1]);
    if(numbytes == 0)
        exit(INTERNAL_ERR);
    printf("%i\n", numbytes);
    char codetext[numbytes];
    strcpy(codetext, p_text);
    int text_position = 0;

    TokenArray MainTokenArray;
    initTokenArray(&MainTokenArray, 50);
    int returnValue;
    
    while(text_position < numbytes){
        // Create new token
        Token New_token;
        Token *p_New_token = &New_token;

        // Call scanning function
        returnValue = scan_token(p_New_token, p_text, &text_position);
        if(returnValue != OK){
            freeTokenArray(&MainTokenArray);
            exit(returnValue);
        }
        insertTokenArray(&MainTokenArray, p_New_token);
    }
    /*
    Token *pointer = &MainTokenArray.array[0];
    if(pointer->ttype == TOKEN_TYPE_PLUS)
      printf("%i\n", pointer->tvalue.whole_num);

    */
    
    freeTokenArray(&MainTokenArray);

}