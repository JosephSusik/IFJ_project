/***************************************
* Project - Compiler for IFJ21
*
* @brief Scan input file and generate tokens
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file parser.c
**************************************/

#include "parser.h"

//get_token, ignores EOL
#define get_token() \
    ({\
        while(1) {\
            parser->exit_code = getNextToken(&parser->token); \
            if (parser->token.ttype == 1) { \
                return PARSER_ERR; \
                break;\
            } \
            if (parser->token.ttype != 2) {\
                if (parser->exit_code != 0) {\
                    return parser->exit_code;\
                    break;\
                }\
                break;\
            }\
        }\
    })\

//get_token, ignores EOL, if EOF -> return 0
#define get_token_EOF_gut() \
    ({\
        while(1) {\
            parser->exit_code = getNextToken(&parser->token); \
            if (parser->token.ttype == 1) { \
                return 0; \
                break;\
            } \
            if (parser->token.ttype != 2) {\
                break;\
            }\
            if (parser->exit_code != 0) { \
                return parser->exit_code; \
                break;\
            } \
        }\
    })\

//get_token, returns EOL if found
#define get_token_EOL_gut() \
    ({ \
    parser->exit_code = getNextToken(&parser->token); \
    if (parser->token.ttype == 1) { \
            return PARSER_ERR; \
        } \
    }) \

#define get_token_EOL_EOF_gut() \
    ({\
        while(1) {\
            parser->exit_code = getNextToken(&parser->token); \
            if (parser->token.ttype == 1) { \
                return 0; \
                break;\
            } \
            if (parser->token.ttype != 2) {\
                return PARSER_ERR; \
                break;\
            }\
            if (parser->token.ttype == 2) {\
                break;\
            }\
            if (parser->exit_code != 0) { \
                return parser->exit_code; \
                break;\
            } \
        }\
    })\


#define token_kword(kword_2) \
    ({\
        if (parser->token.tvalue.kword != kword_2) { \
            return PARSER_ERR; \
        }\
    })\

#define token_utype(utype) \
    ({\
        if (parser->token.tuniontype != utype) { \
            return PARSER_ERR; \
        } \
    })\

#define token_stringcmp(s) \
    ({\
        if (string_cmp(parser->token.tvalue.string, s) != 0) { \
            return PARSER_ERR; \
        } \
    })\

#define token_isID() \
    ({\
        token_utype(3); \
    })\

#define token_ttype(type) \
    ({\
        if (parser->token.ttype != type) { \
            return PARSER_ERR; \
        } \
    })\

#define token_int_num_str() \
    ({ \
        if (parser->token.tvalue.kword != 6 && \
        parser->token.tvalue.kword != 7 && \
        parser->token.tvalue.kword != 8 ) {\
            return PARSER_ERR; \
        }\
    }) \


Parser init_parser() {
    Parser parser = malloc(sizeof(struct parser));
    return parser;
}

int free_parser(Parser parser) {
    free(parser);
    return OK;
}

int parse() {
    Parser parser = init_parser();

    parser->exit_code = require(parser);
    if (parser->exit_code != 0) {
        return parser->exit_code;
    }
    //Call function to print header of IFJcode21 -> .IFJcode21
    print_header();

    //Check \n after require ifj21
    get_token_EOL_EOF_gut();

    //do more parsing
    parser->exit_code = prog(parser);
    if (parser->exit_code != 0) {
        return parser->exit_code;
    }




    
    free_parser(parser);
    return 0;
}

int is_kword(Parser parser, int kword_2) {
    if ((int)parser->token.tvalue.kword != kword_2) {
        return 0;
    } else {
        return 1;
    }
}

int is_utype(Parser parser, int utype) {
    if (parser->token.tuniontype != utype) {
        return 0;
    } else {
        return 1;
    }
}

int is_ttype(Parser parser, int type) {
    if ((int)parser->token.ttype != type) {
        return 0;
    } else {
        return 1;
    }
}

// <require> -> REQUIRE ID 
int require(Parser parser) {
    get_token_EOF_gut();
    token_kword(11);
    get_token();
    token_utype(5);
    token_stringcmp("ifj21");
    return 0;
}//end of require()

// <prog>
int prog(Parser parser) {
    get_token_EOF_gut();
    //FUNCTION
    if (is_kword(parser, 3)) {
        get_token();
        token_isID(); // we found ID after FUNCTION
        get_token();
        token_ttype(14); // we found ( after ID
        parser->exit_code = params(parser); //process <params>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->exit_code = return_type(parser); //process <return_type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        //TO-DO: <body>


    //GLOBAL
    } else if (is_kword(parser, 4)) {
        get_token();
        token_isID(); // we found ID after GLOBAL
        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_kword(3); // we found FUNCTION after :
        get_token();
        token_ttype(14); // we found ( after ID
        //TO-DO: process <type>
    //ID
    } else if (is_utype(parser, 3)) {
        get_token();
        token_ttype(14); // we found ( after ID
        //TO-DO: process <args>
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of prog()

// <params>
int params(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        //TO-DO: no arguments in function -> symtable
        return 0;
    } else if (is_utype(parser, 3)) { // ID
        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_int_num_str(); // is it integer,number or string?
        return params_2(parser);
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of params()

// <params_2>
int params_2(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_ttype(parser, 12)) { // ,
        get_token();
        token_isID(); // we found ID
        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_int_num_str(); // is it integer,number or string?
        parser->exit_code = params_2(parser);
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of params_2()

// <return_type>
int return_type(Parser parser) {
    get_token_EOL_gut();
    if (is_ttype(parser, 2)) { // EOL
        return 0;
    } else if (is_ttype(parser, 20)) { // :
        get_token();
        token_int_num_str(); // is it integer,number or string?
        return return_type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of return_type()

// <return_type_2>
int return_type_2(Parser parser) {
    get_token_EOL_gut();
    if (is_ttype(parser, 2)) { // EOL
        return 0;
    } else if (is_ttype(parser, 12)) {  // ,
        get_token();
        token_int_num_str(); // is it integer,number or string?
        parser->exit_code = return_type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of return_type_2()