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
  a->array = calloc(initialSize, sizeof(Token));
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
  size_t z = 0;
  
  printf("V poli je celkove %li tokenu\n", a->used);
  while(x < a->used){
    if(a->array[x].tuniontype == 3){
      printf("Funkce %s\n", a->array[x].tvalue.string[z]);
      z++;
    }
    x++;
  }
  if(a->used > 0){
    free_memory(a->array[x].tvalue.string);
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
    printf("V kodu je celkove %i znaku\n", numbytes);
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
        printf("Novy token %p\n", p_New_token);

        String string; 
        String *p_string = &string;
        printf("Novy string %p\n", p_string);

        int returnValue = string_init(p_string);
        if(returnValue != OK)
          exit(INTERNAL_ERR);

        // Call scanning function
        returnValue = scan_token(p_New_token, p_text, &text_position, p_string);
        
        if(p_New_token->tuniontype != 3){
          free_memory(p_string);
        }
        else{
          p_New_token->tvalue.string = p_string;
          printf("Token je string\n");
        }

        if(returnValue != OK){
            freeTokenArray(&MainTokenArray);
            exit(returnValue);
        }
        //printf("%s\n", *p_New_token->tvalue.string);
        insertTokenArray(&MainTokenArray, p_New_token);
    }

    freeTokenArray(&MainTokenArray);

}