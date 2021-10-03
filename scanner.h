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
* @file scanner.h
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"

/**
 Reserved keywords go here
*/
typedef enum {
	KEYWORD_DO,
	KEYWORD_ELSE,
    KEYWORD_END,
    KEYWORD_FUNCTION,
    KEYWORD_GLOBAL,
    KEYWORD_IF,
    KEYWORD_LOCAL,
    KEYWORD_NIL,
    KEYWORD_REQUIRE,
    KEYWORD_RETURN,
    KEYWORD_THEN,
    KEYWORD_WHILE
} Keyword;

typedef enum {
	TOKEN_TYPE_START, //Start state
    
    TOKEN_TYPE_EOF, // End of file
	TOKEN_TYPE_EOL, // End of line
	TOKEN_TYPE_EMPTY, // Empty
	TOKEN_TYPE_IDENTIFIER, // Identifier
	TOKEN_TYPE_KEYWORD, // Keyword

    TOKEN_TYPE_LETTER, // letter a-z or A-Z
    TOKEN_TYPE_DIGIT,  // digit 0-9
    
    TOKEN_TYPE_PLUS, // +                       END STATE
    TOKEN_TYPE_MINUS, // -                      END STATE
    TOKEN_TYPE_MULTIPLY, // *                   END STATE
    TOKEN_TYPE_LENGTH,  // #                    END STATE
    TOKEN_TYPE_COMMA,   // ,                    END STATE

    TOKEN_TYPE_LEFT_PAR,    // (                END STATE
    TOKEN_TYPE_RIGHT_PAR,   // )                END STATE

    TOKEN_TYPE_DIVISION,    // /
    TOKEN_TYPE_DIVISION_INT,// //
    TOKEN_TYPE_EQUALS,      // =
    TOKEN_TYPE_EG_ASSIGN,   // ~
    TOKEN_TYPE_COLONS,      // :                END STATE

    TOKEN_TYPE_LESS,        // <
    TOKEN_TYPE_LESS_EQ,     // <=
    TOKEN_TYPE_MORE,        // >
    TOKEN_TYPE_MORE_EQ,     // >=

    TOKEN_TYPE_LINE_COMMENTARY,  // --
    TOKEN_TYPE_BLOCK_COMMENTARY_START, // --[[
    TOKEN_TYPE_BLOCK_COMMENTARY_END, // ]]

    TOKEN_TYPE_STRING,
    TOKEN_TYPE_STRING_START,
    TOKEN_TYPE_STRING_ESCAPE,
    TOKEN_TYPE_STRING_ESCAPE_ONE,
    TOKEN_TYPE_STRING_ESCAPE_TWO,
    TOKEN_TYPE_STRING_ESCAPE_THREE,
    TOKEN_TYPE_STRING_ESCAPE_WRITE

} Token_type;

typedef union {
    int whole_num;
    double dec_num;
    String *string;
    Keyword kword;
} Token_value;

typedef struct {
    Token_type ttype;
    Token_value tvalue;
    int tuniontype;
} Token;

int scan_token(Token *New_token, char *p_text, int *p_text_position);