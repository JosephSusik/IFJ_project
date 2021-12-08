/***************************************
* Project - Compiler for IFJ21
* 
* @brief Library for easier string handeling 
* 
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
* 
* @file my_string.c 
**************************************/
#include <stdlib.h>


#include "my_string.h"
#include "error.h"


#define STRING_LEN_INC 8

int string_init(String *s) {
    
    s->str = (char*) malloc(sizeof(STRING_LEN_INC));
    
    if (s->str == NULL) {
        return INTERNAL_ERR;
    }
    
    s->str[0] = '\0';
    s->length = 0;
    s->alloc_size = STRING_LEN_INC;
    
    return OK;
}

void string_free(String *s) {
    free(s->str);
}

void string_clear(String *s) {
    s->str[0] = '\0';
    s->length = 0;
}

int string_add_char(String *s, char c) {
    
    // Not enough memory, need to reallocate
    if ((s->length + 1) >= s->alloc_size) {
        s->str = (char*) realloc(s->str, s->length + STRING_LEN_INC);
        
        if (s->str == NULL) {
               return INTERNAL_ERR;
        }
        
        s->alloc_size = s->length + STRING_LEN_INC; // New allocated size
    
    }
    
    s->str[s->length] = c;  // Add the new char
    s->length++;    // Increase the length by 1
    s->str[s->length] = '\0';   // Insert '\0' to the end of the string
    
    return OK;
}

int string_cmp(String *s, const char *keyword_string){
    return strcmp(s->str, keyword_string);
}

int string_string_cmp(String *s, String *s2) {
    return strcmp(s->str, s2->str);
}


void string_print(String *s) {
    for(int i = 0; i < s->length; i++) {
        printf("%c", s->str[i]);
    }
}

int string_copy(String *s1, String *s2) {
    int new_length = s2->length;
    if (new_length >= s1->alloc_size) {
        if ((s1->str = (char*) realloc(s1->str, new_length + 1)) == NULL) {
            return INTERNAL_ERR;
        s1->alloc_size = new_length + 1;
        }
    }
    strcpy(s1->str, s2->str);
    s1->length = new_length;
    return OK;
}

int string_strtok(String *s1, const char * d, String *s2) {
    string_copy(s2, s1);
    //string_print(s2);

    char *token;
   
    /* get the first token */
    token = strtok(s2->str, d);
    //get the second token
    token = strtok(NULL, d);


    string_clear(s2); 
 /*
    int new_length = strlen(token);
    if (new_length >= s2->alloc_size) {
        if ((s2->str = (char*) realloc(s2->str, new_length + 1)) == NULL) {
            return INTERNAL_ERR;
        s2->alloc_size = new_length + 1;
        }
    }
    strcpy(s2->str, token);
    s2->length = new_length;
*/
    
    s2->length = (int)strlen(token);
    strcpy(s2->str, token);

    
    return 0;
}
