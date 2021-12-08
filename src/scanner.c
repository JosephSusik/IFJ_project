/***************************************
* Project - Compiler for IFJ21
*
* @brief Scan input file and generate tokens
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file scanner.c
**************************************/

#include "error.h"
#include "scanner.h"

// promenna pro ulozeni vstupniho souboru
FILE *source_file;
String *source_string;

void setSourceFile(FILE *f) {
    source_file = f;
}

void setString(String *string) {
    source_string = string;
}

static int free_memory(int exit_code, String *string) {
    string_free(string);
    return exit_code;
}

static int integer_number(String *string, Token *token) {
    char *ptr;
    int integer = (int) strtol(string->str, &ptr, 10);
    if (*ptr) {
        return free_memory(INTERNAL_ERR, string);
    }
    token->tvalue.whole_num = integer;
    token->ttype = TOKEN_TYPE_INT;
    token->tuniontype = 1;
    return free_memory(OK, string);
}

static int double_number(String *string, Token *token) {
    char *ptr;
    double double_number = strtod(string->str, &ptr);
    if (*ptr) {
        return free_memory(INTERNAL_ERR, string);
    }
    token->tvalue.dec_num = double_number;
    token->ttype = TOKEN_TYPE_DOUBLE;
    token->tuniontype = 2;
    return free_memory(OK, string);
}

int is_string_keyword(String *string, Token *token) {
    if(string_cmp(string, "do") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_DO;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "else") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_ELSE;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "end") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_END;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "function") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_FUNCTION;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "global") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_GLOBAL;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "if") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_IF;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "integer") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_INTEGER;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "local") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_LOCAL;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "nil") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_NIL;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "number") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_NUMBER;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "require") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_REQUIRE;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "return") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_RETURN;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "string") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_STRING;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "then") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_THEN;
        token->tuniontype = 4;
    }
    else if(string_cmp(string, "while") == 0){
        token->ttype = TOKEN_TYPE_KEYWORD;
        token->tvalue.kword = KEYWORD_WHILE;
        token->tuniontype = 4;
    }
    else{
        token->ttype = TOKEN_TYPE_IDENTIFIER;
        token->tuniontype = 3;
        if(string_copy(token->tvalue.string, string) == 1){
            return free_memory(INTERNAL_ERR, string);
        }
    }
    return OK;
}

int getNextToken(Token *token) {
    //Problem with source file
    if (source_file == NULL) {
        return INTERNAL_ERR;
    }
    Token_type State = TOKEN_TYPE_START;

    token->ttype = TOKEN_TYPE_EMPTY;
    token->tuniontype = 0;
    token->tvalue.string = source_string;

    String tmp_string;
    String *string_2 = &tmp_string;
    
    if (string_init(string_2) != 0) {
        return INTERNAL_ERR;
    }

    char c;
    char escape_one;
    char escape_two;
    char escape_three;
    int escape_number;

    while(1) {
        c = (char)getc(source_file);
        switch(State) {
            case TOKEN_TYPE_START:
                if (c == '\n') {                        // newline
                    State = TOKEN_TYPE_EOL;
                } else if (isspace(c) || (c == '\t')) { //space or tab
                    token->ttype = TOKEN_TYPE_EMPTY; 
                    //printf("space\n");
                    return free_memory(OK, string_2);
                    //State = TOKEN_TYPE_START;
                } else if (c == EOF) {                  //end of file
                    token->ttype = TOKEN_TYPE_EOF; 
                    return free_memory(OK, string_2);   
                } else if (c == '<') {                  // <
                    State = TOKEN_TYPE_LESS;
                } else if (c == '>') {                  // > 
                    State = TOKEN_TYPE_MORE;
                } else if (c == '=') {                  // =
                    State = TOKEN_TYPE_ASSIGN;//TOKEN_TYPE_EQUALS;
                } else if (c == '+') {                  // +
                    token->ttype = TOKEN_TYPE_PLUS;
                    return free_memory(OK, string_2);
                } else if (c == '-') {                  // -
                    State = TOKEN_TYPE_MINUS;
                } else if (c == '*') {                  // *
                    token->ttype = TOKEN_TYPE_MULTIPLY;
                    return free_memory(OK, string_2);
                } else if (c == '/') {                  // /
                    State = TOKEN_TYPE_DIVISION;
                } else if (c == '"') {                  // "
                    State = TOKEN_TYPE_STRING_START;
                } else if (c == ':') {                  // :
                    token->ttype = TOKEN_TYPE_COLONS;
                    return free_memory(OK, string_2);
                } else if (c == '#') {                  // #
                    token->ttype = TOKEN_TYPE_LENGTH;
                    return free_memory(OK, string_2);
                } else if (c == '~') {                  // ~
                    State = TOKEN_TYPE_EG_ASSIGN;
                } else if (c == ',') {                  // ,
                    token->ttype = TOKEN_TYPE_COMMA;
                    return free_memory(OK, string_2);
                } else if (c == '_') {                  // _
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_IDENTIFIER;
                } else if (c == '(') {                  // (
                    token->ttype = TOKEN_TYPE_LEFT_PAR;
                    return free_memory(OK, string_2);
                } else if (c == ')') {                  // )
                    token->ttype = TOKEN_TYPE_RIGHT_PAR;
                    return free_memory(OK, string_2);
                } else if (isalpha(c)) {                // a-z
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_IDENTIFIER;
                } else if (isdigit(c)) {                // 0-9
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_DIGIT;
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
                break;
            /******** END OF TOKE_TYPE_START ********/
            case TOKEN_TYPE_LESS:
                if (c == '=') {
                    token->ttype = TOKEN_TYPE_LESS_EQ;
                } else {
                    ungetc(c, source_file);
                    token->ttype = TOKEN_TYPE_LESS;
                }
                return free_memory(OK, string_2);
            
            case TOKEN_TYPE_MORE:
                if (c == '=') {
                    token->ttype = TOKEN_TYPE_MORE_EQ;
                } else {
                    ungetc(c, source_file);
                    token->ttype = TOKEN_TYPE_MORE;
                }
                return free_memory(OK, string_2);
            
            case TOKEN_TYPE_ASSIGN://TOKEN_TYPE_EQUALS:
                if (c == '=') {
                    token->ttype = TOKEN_TYPE_EQUALS;
                } else {
                    ungetc(c, source_file);
                    token->ttype = TOKEN_TYPE_ASSIGN;
                }
                return free_memory(OK, string_2);

            case TOKEN_TYPE_EG_ASSIGN:
                if (c == '=') {
                    token->ttype = TOKEN_TYPE_EG_ASSIGN;
                    return free_memory(OK, string_2);
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
            
            case TOKEN_TYPE_DIVISION:
                if (c == '/') {
                    token->ttype = TOKEN_TYPE_DIVISION_INT;
                } else {
                    ungetc(c, source_file);
                }
                return free_memory(OK, string_2);
            
            case TOKEN_TYPE_STRING_START:
                if (c < 32) {
                    return free_memory(SCANNER_ERR, string_2);
                } 
                if (c == '"') {
                    c = (char)getc(source_file);
                    if (isspace(c) || c == ')' || c == ','|| c == '-') {
                        ungetc(c, source_file);
                        token->ttype = TOKEN_TYPE_STRING;
                        token->tuniontype = 5;
                        if(string_copy(token->tvalue.string, string_2) == 1){
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else {
                        return free_memory(SCANNER_ERR, string_2);
                    }
                } else if (c == '\\') {
                    State = TOKEN_TYPE_STRING_ESCAPE;
                } else {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                }
                break;

            case TOKEN_TYPE_STRING_ESCAPE:
                if (c < 32) {
                    return free_memory(SCANNER_ERR, string_2);
                }
                if (c == 'n') {
                    if (string_add_char(string_2, '\n') != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_STRING_START;
                } else if (c == '\\') {
                    if (string_add_char(string_2, '\\') != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_STRING_START;
                } else if (c == 't') {
                    if (string_add_char(string_2, '\t') != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_STRING_START;
                } else if (isdigit(c)) {
                    escape_one = c;
                    State = TOKEN_TYPE_STRING_ESCAPE_TWO;
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
                break;

            case TOKEN_TYPE_STRING_ESCAPE_TWO:
                if (isdigit(c)) {
                    escape_two = c;
                    State = TOKEN_TYPE_STRING_ESCAPE_THREE;
                } else {
                    return free_memory(SCANNER_ERR,  string_2);
                }
                break;

            case TOKEN_TYPE_STRING_ESCAPE_THREE:
                if (isdigit(c)) {
                    escape_three = c;
                    State = TOKEN_TYPE_STRING_ESCAPE_WRITE;
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
                break;

            case TOKEN_TYPE_STRING_ESCAPE_WRITE:
                escape_number = (escape_one - 48) * 100 + (escape_two - 48) * 100 + (escape_three - 48) * 100;
                if (string_add_char(string_2, (char)escape_number) != 0) {
                    return free_memory(INTERNAL_ERR, string_2);
                }
                State = TOKEN_TYPE_STRING_START;
                break;

            case TOKEN_TYPE_DIGIT:
                if (isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_DIGIT;                    
                } else if (c == '.') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_DOUBLE_DOT;
                } else if (c == 'e'|| c == 'E') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT;
                } else if (isalpha(c)) {
                    return free_memory(SCANNER_ERR, string_2);
                } else {
                    ungetc(c, source_file);
                    return integer_number(string_2, token);
                }
                break;

            case TOKEN_TYPE_DOUBLE_DOT:
                if (isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_DOUBLE;
                } else if (c == 'e'|| c == 'E') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT;
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
                break;
            
            case TOKEN_TYPE_DOUBLE:
                if (isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_DOUBLE;
                } else if (c == 'e' || c == 'E') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT;
                } else {
                    ungetc(c, source_file);
                    return double_number(string_2, token);
                }
                break;
            
            case TOKEN_TYPE_EXPONENT:
                if(isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                } else if (c == '+' || c == '-') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT_SIGN;
                }
                break;
            
            case TOKEN_TYPE_EXPONENT_SIGN:
                if(isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                } else {
                    return free_memory(SCANNER_ERR, string_2);
                }
                break;

            case TOKEN_TYPE_EXPONENT_EXPONENT:
                if(isdigit(c)) {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_EXPONENT_EXPONENT;
                } else {
                    ungetc(c, source_file);
                    return double_number(string_2, token);
                }
                break;

            case TOKEN_TYPE_IDENTIFIER:
                if (isalpha(c)  || isdigit(c) || c == '_') {
                    if (string_add_char(string_2, c) != 0) {
                        return free_memory(INTERNAL_ERR, string_2);
                    }
                    State = TOKEN_TYPE_IDENTIFIER;
                } else {
                    if (isspace(c)) {
                        ungetc(c, source_file); 
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2); 
                    } else if (c == '(') {
                        ungetc(c, source_file);
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else if (c == ')') {
                        ungetc(c, source_file);
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else if (c == ':') {
                        ungetc(c, source_file);
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else if (c == ',') {
                        ungetc(c, source_file);
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else if (c == '-') {
                        ungetc(c, source_file);
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else if (c == EOF) {
                        ungetc(c, source_file); 
                        if(is_string_keyword(string_2, token) != 0) {
                            return free_memory(INTERNAL_ERR, string_2);
                        }
                        return free_memory(OK, string_2);
                    } else {
                        return free_memory(SCANNER_ERR, string_2);
                    }
                }
                break;
            
            case TOKEN_TYPE_MINUS:
                if (c == '-') {
                    State = TOKEN_TYPE_LINE_COMMENTARY;
                } else {
                    ungetc(c, source_file); 
                    token->ttype = TOKEN_TYPE_MINUS;
                    return free_memory(OK, string_2);
                }
                break;
            
            case TOKEN_TYPE_LINE_COMMENTARY:
                if (c == '\n') { 
                    State = TOKEN_TYPE_START;
                    ungetc(c, source_file);
                    break;
                } else if (c == '['){
                    State = TOKEN_TYPE_BLOCK_COMMENTARY_START;
                    break;
                } else if (c == EOF) {
                    token->ttype = TOKEN_TYPE_EOF; 
                    return free_memory(OK, string_2);
                    break;
                }
                State = TOKEN_TYPE_LINE_COMMENTARY;
                break;

            case TOKEN_TYPE_BLOCK_COMMENTARY_START:
                if (c == '[') {
                    State = TOKEN_TYPE_BLOCK_COMMENTARY_END;
                } else {
                    State = TOKEN_TYPE_LINE_COMMENTARY;
                }
                break;
            
            case TOKEN_TYPE_BLOCK_COMMENTARY_END:
                if (c == ']') {  
                    State = TOKEN_TYPE_BLOCK_COMMENTARY_END_2;
                } else {
                    State = TOKEN_TYPE_BLOCK_COMMENTARY_END;
                }
                break;

            case TOKEN_TYPE_BLOCK_COMMENTARY_END_2:
                if (c == ']') {
                    State = TOKEN_TYPE_START;
                    break;
                } else {
                    State = TOKEN_TYPE_BLOCK_COMMENTARY_END;
                }
                break;

            case TOKEN_TYPE_EOL:
                if (isspace(c)) {
                    break;
                }
                ungetc(c, source_file);
                token->ttype = TOKEN_TYPE_EOL;
                return free_memory(OK, string_2);
            
            case TOKEN_TYPE_EOF: //
            case TOKEN_TYPE_EMPTY: // 
            case TOKEN_TYPE_KEYWORD:
            case TOKEN_TYPE_LETTER:
            case TOKEN_TYPE_PLUS:
            case TOKEN_TYPE_MULTIPLY:
            case TOKEN_TYPE_LENGTH:
            case TOKEN_TYPE_COMMA:
            case TOKEN_TYPE_INT:
            case TOKEN_TYPE_LEFT_PAR:
            case TOKEN_TYPE_RIGHT_PAR:
            case TOKEN_TYPE_DIVISION_INT:
            case TOKEN_TYPE_COLONS:
            case TOKEN_TYPE_LESS_EQ: //
            case TOKEN_TYPE_MORE_EQ: // 
            case TOKEN_TYPE_STRING:
            case TOKEN_TYPE_STRING_ESCAPE_ONE:
            case TOKEN_TYPE_EQUALS://TOKEN_TYPE_ASSIGN:
                break;
        } //End of switch(state)
    } //End of while(1)
}//End of getNextToken()
