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

static int integer_number(String *str, Token *New_token){
    char *ptr;
    int integer = (int)strtol(str->str, &ptr, 10);
    New_token->tvalue.whole_num = integer;
    New_token->ttype = TOKEN_TYPE_INT;
    New_token->tuniontype = 1;
    return OK;
}

static int double_number(String *str, Token *New_token){
    char *ptr;
    double double_number = strtod(str->str, &ptr);
    New_token->tvalue.dec_num = double_number;
    New_token->ttype = TOKEN_TYPE_DOUBLE;
    return OK;
}

int is_string_keyword(String *s, Token *token){
    if(!string_cmp(s, "do")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_DO;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "else")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_ELSE;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "end")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_END;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "function")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_FUNCTION;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "global")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_GLOBAL;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "if")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_IF;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "integer")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_INTEGER;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "local")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_LOCAL;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "nil")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_NIL;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "number")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_NUMBER;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "require")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_REQUIRE;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "return")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_RETURN;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "string")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_STRING;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "then")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_THEN;
        token->tuniontype = 4;
    }
    else if(!string_cmp(s, "while")){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_WHILE;
        token->tuniontype = 4;
    }
    else{
        token->ttype = TOKEN_TYPE_IDENTIFIER;
        token->tuniontype = 3;
    }
    return OK;
}

int scan_token(Token *New_token, char *p_text, int *p_text_position, String *str){
    Token_type State = TOKEN_TYPE_START;

    //printf("%i %c\n", *p_text_position, p_text[*p_text_position]);
    //inc_position(p_text_position);

    New_token->ttype = TOKEN_TYPE_EMPTY;
    New_token->tuniontype = 0;

    char c;
    char escape_one;
    char escape_two;
    char escape_three;
    int escape_number;
    int returnValue;

    while(1){
        c = p_text[*p_text_position];
        printf("Scan %c - %i\n", p_text[*p_text_position], p_text[*p_text_position]);
        switch(State){
            /*          ========== START OF START SWITCH ==========        */
            case TOKEN_TYPE_START:
                if((c == ' ') || (c == '\t')){  // space or tab
                    State = TOKEN_TYPE_START;
                }
                else if(c == '\n'){     // new line
                    New_token->ttype = TOKEN_TYPE_EOL;
                    inc_position(p_text_position);
                    return OK;
                }
                else if(c == EOF){      // end of file
                    New_token->ttype = TOKEN_TYPE_EOF;
                    inc_position(p_text_position);
                    return OK;
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
                    return OK;
                }
                else if(c == '-'){      // -
                    New_token->ttype = TOKEN_TYPE_MINUS;
                    inc_position(p_text_position);
                    return OK;
                }
                else if(c == '*'){      // *
                    New_token->ttype = TOKEN_TYPE_MULTIPLY;
                    inc_position(p_text_position);
                    return OK;
                }
                else if(c == '/'){      // /
                    State = TOKEN_TYPE_DIVISION;
                }
                else if(c == '"'){      // "
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == '#'){      // #
                    New_token->ttype = TOKEN_TYPE_LENGTH;
                    inc_position(p_text_position);
                    return OK;
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
                    return OK;
                }
                else if(c == ','){      // ,
                    New_token->ttype = TOKEN_TYPE_COMMA;
                    inc_position(p_text_position);
                    return OK;
                }
                else if((c == '_')){      // _
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    State = TOKEN_TYPE_IDENTIFIER;
                }
                else if(c == '('){      // (
                    New_token->ttype = TOKEN_TYPE_LEFT_PAR;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_LEFT_PAR;
                    return OK;
                }
                else if(c == ')'){      // )
                    New_token->ttype = TOKEN_TYPE_RIGHT_PAR;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_RIGHT_PAR;
                    return OK;
                }
                else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){      // is letter a-z or A-Z
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        inc_position(p_text_position);
                        return INTERNAL_ERR;
                    }
                    State = TOKEN_TYPE_LETTER;
                }
                else if(c >= '0' && c <= '9'){      // is digit 0-9
                    State = TOKEN_TYPE_DIGIT;
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                }
                else{   // error 1
                    return SCANNER_ERR;
                }
                inc_position(p_text_position);
                break;
            /*      ========== END OF START SWITCH ==========     */

            /*      ========== START OF LESS AND MORE ==========      */
            case TOKEN_TYPE_LESS: // <
                if(c == '='){   // <=
                    New_token->ttype = TOKEN_TYPE_LESS_EQ;
                    inc_position(p_text_position);
                    return OK;
                }
                else{   // <
                    New_token->ttype = TOKEN_TYPE_LESS;
                    return OK;
                }
                break;
            case TOKEN_TYPE_MORE: // >
                if(c == '='){   // >=
                    New_token->ttype = TOKEN_TYPE_MORE_EQ;
                    inc_position(p_text_position);
                    return OK;
                }
                else{   // >
                    New_token->ttype = TOKEN_TYPE_MORE;
                    return OK;
                }
                break;
            /*      ========== END OF LESS AND MORE ==========      */

            /*      ========== START OF EQUALS ==========      */
            case TOKEN_TYPE_EG_ASSIGN: // ~
                if(c == '='){   // ~=
                    New_token->ttype = TOKEN_TYPE_EG_ASSIGN;
                    inc_position(p_text_position);
                    return OK;
                }
                else{   
                    return SCANNER_ERR;
                }
                break;

            case TOKEN_TYPE_EQUALS: // =
                if(c == '='){   // ==
                    New_token->ttype = TOKEN_TYPE_EQUALS;
                    inc_position(p_text_position);
                    return OK;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            /*      ========== END OF EQUALS ==========      */

            /*      ========== START OF ARYTMETIC OPERATIONS ==========      */
            case TOKEN_TYPE_DIVISION: // /
                if(c == '/'){
                    New_token->ttype = TOKEN_TYPE_DIVISION_INT;
                    inc_position(p_text_position);
                    return OK;
                }
                else{
                    New_token->ttype = TOKEN_TYPE_DIVISION;
                    return OK;
                }
                break;

            /*      ========== END OF ARYTMETIC OPERATIONS ==========      */

            /*      ========== START OF STRING ==========      */
            case TOKEN_TYPE_STRING_START:
                if(c < 32){
                    return SCANNER_ERR;
                }
                else if(c == '"'){
                    
                    New_token->ttype = TOKEN_TYPE_STRING;
                    New_token->tuniontype = 3;

                    inc_position(p_text_position);
                    
                    return OK;
                }
                else if(c == '\\'){
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE;
                }
                else{
                    returnValue = string_add_char(str, c);
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE:
                if(c < 32){
                    return SCANNER_ERR;
                }
                else if(c == 'n'){
                    returnValue = string_add_char(str, '\\n');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == '\\'){
                    returnValue = string_add_char(str, '\\');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c == 't'){
                    returnValue = string_add_char(str, '\t');
                    inc_position(p_text_position);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    State = TOKEN_TYPE_STRING_START;
                }
                else if(c >= '0' && c <= '9'){
                    escape_one = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_TWO;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_TWO:
                if(c >= '0' && c <= '9'){
                    escape_two = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_THREE;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_THREE:
                if(c >= '0' && c <= '9'){
                    escape_three = c;
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_STRING_ESCAPE_WRITE;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            case TOKEN_TYPE_STRING_ESCAPE_WRITE:
                escape_number = (escape_one - 48) * 100  + (escape_two - 48) * 10 + (escape_three - 48);
                returnValue = string_add_char(str, (char)escape_number);
                if(returnValue == INTERNAL_ERR){
                    return INTERNAL_ERR;
                }
                State = TOKEN_TYPE_STRING_START;
                break;
            /*      ========== END OF STRING ==========      */

            /*      ========== START OF NUMBER ==========      */
            case TOKEN_TYPE_DIGIT:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_DIGIT;
                }
                else if(c == '.'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_DOUBLE_DOT;
                }
                else if(c == 'e' || c == 'E'){
                    returnValue = string_add_char(str, 'e');
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT;
                }
                else{
                    return integer_number(str, New_token);
                }
                break;
            case TOKEN_TYPE_DOUBLE_DOT:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_DOUBLE;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            case TOKEN_TYPE_DOUBLE:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_DOUBLE;
                }
                else if(c == 'e' || c == 'E'){
                    returnValue = string_add_char(str, 'e');
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT;
                }
                else{
                    return double_number(str, New_token);
                }
                break;
            case TOKEN_TYPE_EXPONENT:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                }
                else if(c == '+' || c == '-'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT_SIGN;
                }
                break;
            case TOKEN_TYPE_EXPONENT_SIGN:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                }
                else{
                    return SCANNER_ERR;
                }
                break;
            case TOKEN_TYPE_EXPONENT_EXPONENT:
                if(c >= '0' && c <= '9'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                }
                else{
                    return double_number(str, New_token);
                }
                break;
            /*      ========== END OF NUMBER ==========      */

            /*      ========== START OF IDENTIFICATOR/KEYWORD ==========      */
            case TOKEN_TYPE_IDENTIFIER:
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_IDENTIFIER;
                }
                else{
                    New_token->ttype = TOKEN_TYPE_IDENTIFIER;
                    return OK;
                }
                break;
            case TOKEN_TYPE_LETTER:
                if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_LETTER;
                }
                else if(c == '_'){
                    returnValue = string_add_char(str, c);
                    if(returnValue == INTERNAL_ERR){
                        return INTERNAL_ERR;
                    }
                    inc_position(p_text_position);
                    State = TOKEN_TYPE_IDENTIFIER;
                }
                else{
                    return is_string_keyword(str, New_token);
                }
                break;
            case TOKEN_TYPE_EOF:
            case TOKEN_TYPE_EOL:
            case TOKEN_TYPE_EMPTY:
            case TOKEN_TYPE_KEYWORD:
            case TOKEN_TYPE_PLUS:
            case TOKEN_TYPE_MINUS:
            case TOKEN_TYPE_MULTIPLY:
            case TOKEN_TYPE_LENGTH:
            case TOKEN_TYPE_COMMA:
            case TOKEN_TYPE_INT:
            case TOKEN_TYPE_LEFT_PAR:
            case TOKEN_TYPE_RIGHT_PAR:
            case TOKEN_TYPE_DIVISION_INT:
            case TOKEN_TYPE_COLONS:
            case TOKEN_TYPE_LESS_EQ:
            case TOKEN_TYPE_MORE_EQ:
            case TOKEN_TYPE_LINE_COMMENTARY:
            case TOKEN_TYPE_BLOCK_COMMENTARY_START:
            case TOKEN_TYPE_BLOCK_COMMENTARY_END:
            case TOKEN_TYPE_STRING:
            case TOKEN_TYPE_STRING_ESCAPE_ONE:
                break;

            /*      ========== END OF IDENTIFICATOR/KEYWORD ==========      */

        }
    }
}