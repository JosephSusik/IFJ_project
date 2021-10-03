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
* @file scanner.c 
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "error.h"

void inc_position(int *position){
    *position = *position + 1;
}

int scan_token(Token *New_token, char *p_text, int *p_text_position){
    Token_type State = TOKEN_TYPE_START;

    //printf("%i %c\n", *p_text_position, p_text[*p_text_position]);
    //inc_position(p_text_position);

    New_token->ttype = TOKEN_TYPE_EMPTY;

    String string;
    String *str = &string;

    int returnValue = string_init(str);
    if(returnValue != OK)
        return free_memory(INTERNAL_ERR, str);
    char c;
    char escape_one;
    char escape_two;
    char escape_three;
    int escape_number;

    while(1){
        c = p_text[*p_text_position];
        switch(State){
            /*          ========== START OF START SWITCH ==========        */
            case TOKEN_TYPE_START:
                if((c == ' ') || (c == '\t')){  // space or tab
                    State = TOKEN_TYPE_START;
                }
                else if(c == '\n'){     // new line
                    New_token->ttype = TOKEN_TYPE_EOL;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == EOF){      // end of file
                    New_token->ttype = TOKEN_TYPE_EOF;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '<'){      // <
                    State = TOKEN_TYPE_LESS;
                }
                else if(c == '>'){      // >
                    State = TOKEN_TYPE_MORE;
                }
                else if(c == '+'){      // +
                    New_token->ttype = TOKEN_TYPE_PLUS;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '-'){      // -
                    New_token->ttype = TOKEN_TYPE_MINUS;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '*'){      // *
                    New_token->ttype = TOKEN_TYPE_MULTIPLY;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '/'){      // /
                    State = TOKEN_TYPE_DIVISION;
                }
                else if(c == '"'){      // "
                    State = TOKEN_TYPE_STRING_START;
                    inc_position(p_text_position);
                }
                else if(c == '#'){      // #
                    New_token->ttype = TOKEN_TYPE_LENGTH;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '='){      // =
                    State = TOKEN_TYPE_EQUALS;
                }
                else if(c == '~'){      // ~
                    State = TOKEN_TYPE_EG_ASSIGN;
                }
                else if(c == ':'){      // :
                    New_token->ttype = TOKEN_TYPE_COLONS;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == ','){      // ,
                    New_token->ttype = TOKEN_TYPE_COMMA;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if((c == '_') || (c >= 'A' && c <= 'Z')){      // _
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        inc_position(p_text_position);
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_IDENTIFIER;
                }
                else if(c == '('){      // (
                    New_token->ttype = TOKEN_TYPE_LEFT_PAR;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == ')'){      // )
                    New_token->ttype = TOKEN_TYPE_RIGHT_PAR;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if((c >= 'a' && c <= 'z')){      // is letter a-z or A-Z
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        inc_position(p_text_position);
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_LETTER;
                }
                else if(c >= '0' && c <= '9'){      // is digit 0-9
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        inc_position(p_text_position);
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_DIGIT;
                }
                else{   // error 1
                    inc_position(p_text_position);
                    return free_memory(SCANNER_ERR, str);
                }
                inc_position(p_text_position);
                break;
            /*      ========== END OF START SWITCH ==========     */

            /*      ========== START OF LESS AND MORE ==========      */
            case TOKEN_TYPE_LESS: // <
                if(c == '='){   // <=
                    New_token->ttype = TOKEN_TYPE_LESS_EQ;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else{   // <
                    New_token->ttype = TOKEN_TYPE_LESS;
                    return free_memory(OK, str);
                }
                break;
            case TOKEN_TYPE_MORE: // >
                if(c == '='){   // >=
                    New_token->ttype = TOKEN_TYPE_MORE_EQ;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else{   // >
                    New_token->ttype = TOKEN_TYPE_MORE;
                    return free_memory(OK, str);
                }
                break;
            /*      ========== END OF LESS AND MORE ==========      */

            /*      ========== START OF EQUALS ==========      */
            case TOKEN_TYPE_EG_ASSIGN: // ~
                if(c == '='){   // ~=
                    New_token->ttype = TOKEN_TYPE_EG_ASSIGN;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else{   
                    return free_memory(SCANNER_ERR, str);
                }
                break;

            case TOKEN_TYPE_EQUALS: // =
                if(c == '='){   // ==
                    New_token->ttype = TOKEN_TYPE_EQUALS;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else{
                    return free_memory(SCANNER_ERR, str);
                }
                break;
            /*      ========== END OF EQUALS ==========      */

            /*      ========== START OF ARYTMETIC OPERATIONS ==========      */
            case TOKEN_TYPE_DIVISION: // /
                if(c == '/'){
                    New_token->ttype = TOKEN_TYPE_DIVISION_INT;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else{
                    New_token->ttype = TOKEN_TYPE_DIVISION;
                    return free_memory(OK, str);
                }
                break;

            /*      ========== END OF ARYTMETIC OPERATIONS ==========      */

            /*      ========== START OF STRING ==========      */
            case TOKEN_TYPE_STRING_START:
                if(c < 32){
                    return free_memory(SCANNER_ERR, str);
                }
                else if(c == '"'){
                    
                    /* JESTE SE SEM PODIVAT POZDEJI */
                    New_token->ttype = TOKEN_TYPE_STRING;
                    New_token->tuniontype = 3;
                    inc_position(p_text_position);
                    return free_memory(OK, str);
                }
                else if(c == '\\'){
                    State = TOKEN_TYPE_STRING_ESCAPE;
                }
                else{
                    returnValue = string_add_char(str, c);
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return free_memory(INTERNAL_ERR, str);
                    }
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE:
                if(c < 32){
                    return free_memory(SCANNER_ERR, str);
                }
                else if(c == '"'){
                    returnValue = string_add_char(str, '"');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == 'n'){
                    returnValue = string_add_char(str, 'n');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == '\\'){
                    returnValue = string_add_char(str, '\\');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == 't'){
                    returnValue = string_add_char(str, 't');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return free_memory(INTERNAL_ERR, str);
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c >= '0' && c <= '9'){
                    escape_one = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_TWO;
                }
                else{
                    return free_memory(SCANNER_ERR, str);
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_TWO:
                if(c >= '0' && c <= '9'){
                    escape_two = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_THREE;
                }
                else{
                    return free_memory(SCANNER_ERR, str);
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_THREE:
                if(c >= '0' && c <= '9'){
                    escape_three = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_WRITE;
                }
                else{
                    return free_memory(SCANNER_ERR, str);
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_WRITE:
                escape_number = (escape_one - 48) * 100  + (escape_two - 48) * 10 + (escape_three - 48);
                returnValue = string_add_char(str, (char)escape_number);
                if(returnValue == INTERNAL_ERR){
                    return free_memory(INTERNAL_ERR, str);
                }
                State = TOKEN_TYPE_STRING_START;
                break;
            /*      ========== END OF STRING ==========      */

        }
    }
}