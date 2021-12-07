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

//get_token, ignores EOL, ignores space, tab
#define get_token() \
    ({\
        while(1) {\
            parser->exit_code = getNextToken(&parser->token); \
            if (parser->token.ttype == 1) { \
                return PARSER_ERR; \
                break;\
            } \
            if ((parser->token.ttype != 2) && (parser->token.ttype != 3)) {\
                if (parser->exit_code != 0) {\
                    return parser->exit_code;\
                    break;\
                }\
                break;\
            }\
            if (parser->exit_code != 0) {\
                    return parser->exit_code;\
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
            if ((parser->token.ttype != 2) && (parser->token.ttype != 3)) {\
                if (parser->exit_code != 0) { \
                    return parser->exit_code; \
                    break;\
                } \
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
    while(1) {\
        parser->exit_code = getNextToken(&parser->token); \
        if (parser->token.ttype == 1) { \
            return PARSER_ERR; \
            break;\
        } \
        if (parser->token.ttype != 3) {\
            if (parser->exit_code != 0) { \
                return parser->exit_code; \
                break;\
            } \
        break;\
        }\
    }\
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

#define token_int_num_str_nil() \
    ({ \
        if (parser->token.tvalue.kword != 6 && \
        parser->token.tvalue.kword != 7 && \
        parser->token.tvalue.kword != 8 &&\
        parser->token.tvalue.kword != 10) {\
            return PARSER_ERR; \
        }\
    }) \


Parser init_parser() {
    Parser parser = malloc(sizeof(struct parser));
    parser->_if = 0;
    parser->_else = 0;
    parser->_while = 0;
    parser->end = false;
    parser->wend = false;
    symtable_init(&parser->global_symtable);
    string_init(&parser->func_id);
    parser->num_params = 0;
    parser->num_return = 0;
    parser->num_types = 0;
    stack_init(&parser->tmp_stack);
    stack_init(&parser->tmp_stack_2);
    string_init(&parser->tmp_string);
    return parser;
}

int free_parser(Parser parser) {
    symtable_dispose(&parser->global_symtable);
    string_free(&parser->func_id);
    stack_dispose(&parser->tmp_stack);
    //stack_dispose(&parser->tmp_stack_2);
    string_free(&parser->tmp_string);
    free(parser);
    return OK;
}

int parse() {
    Parser parser = init_parser();

    /*require "ifj21" is not needed*/
    parser->exit_code = require(parser);
    if (parser->exit_code != 0) {
        return parser->exit_code;
    }
    
    
    //require(parser);
    
    //Call function to print header of IFJcode21 -> .IFJcode21
    print_header();

    parser->exit_code = prog(parser);
    if (parser->exit_code != 0) {
        return parser->exit_code;
    }



    
    free_parser(parser);

    return 0;
}

int is_kword(Parser parser, Keyword kword_2) {
    if (parser->token.tvalue.kword != kword_2) {
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

int is_int_num_str(Parser parser) {
    if (parser->token.tvalue.kword != 6 && 
        parser->token.tvalue.kword != 7 && 
        parser->token.tvalue.kword != 8 ) {
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
        //Check it function is defined
        string_copy(&parser->func_id,parser->token.tvalue.string);
        nodeptr a = symtable_search(&parser->global_symtable, &parser->func_id);
        
        if(a != NULL) {
            if (a->func_data->defined == true) {
                return UNDEFINED_VAR_ERR;
            }
        }


        stack nul;
        stack_init(&nul);
    

        //stack_push(&parser->params_stack, parser->token.tvalue.string + type);
        get_token();
        token_ttype(14); // we found ( after ID
        parser->exit_code = params(parser); //process <params>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

        /*
        -load return params into stack
        */
        parser->exit_code = return_type(parser); //process <return_type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        
        if(a != NULL) {
            if (a->func_data->declared == true) {
                //compare stacks from a->data->top.data a parser->tmp_stack->data
                //a->func_data->stack_params.top->data == parser->tmp_stack.top->data; //name:type
                //if same, somehow reassign values to symtable_insert bellow
                string_strtok(&parser->tmp_stack.top->data, ":", &parser->tmp_string);
                //string_print(&parser->tmp_string);
            }
        }



        symtable_insert(&parser->global_symtable, &parser->func_id, func, true, true, parser->num_params, parser->num_return, parser->tmp_stack, parser->tmp_stack_2);

        //THIS IS HOW U ACCESS NODE WITH NAME
        //nodeptr b = symtable_search(&parser->global_symtable, &parser->func_id);
      
    
        
        parser->exit_code = body(parser); //process <body>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->exit_code = prog(parser); //process <prog>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

    //GLOBAL
    } else if (is_kword(parser, 4)) {
        get_token();
        token_isID(); // we found ID after GLOBAL

        string_copy(&parser->func_id,parser->token.tvalue.string);
        
        nodeptr a = symtable_search(&parser->global_symtable, &parser->func_id);
        
        if(a != NULL) {
            if (a->func_data->declared == true) {
                return UNDEFINED_VAR_ERR;
            }
        }


        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_kword(3); // we found FUNCTION after :
        get_token();
        token_ttype(14); // we found ( after ID

        parser->exit_code = type(parser); //process <type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

        parser->exit_code = return_type(parser); //process <return_type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

        symtable_insert(&parser->global_symtable, &parser->func_id, func, true, false, parser->num_types, parser->num_return, parser->tmp_stack, parser->tmp_stack_2);

        parser->exit_code = prog(parser); //process <prog>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    
    //ID
    } else if (is_utype(parser, 3)) {
        if (string_cmp(parser->token.tvalue.string, "write") == 0) {
            get_token();
            if (is_ttype(parser, 14)) {
                while(1) {
                //get_token();
                getNextToken(&parser->token);
                //isID()
                //search-bintree -> ID value
                //push_stack
                if (is_ttype(parser, 15)) {
                    //parser->exit_code = eval(stack)
                    // if (exit_code != 0) return exitcode
                    break;
                }
                }
            } else {
                return PARSER_ERR;
            }
            parser->exit_code = prog(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }
            return 0;
        }
        
        get_token();
        token_ttype(14); // we found ( after ID
        parser->exit_code = args(parser); //process <args>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        
        parser->exit_code = prog(parser); //process <prog>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    
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
        string_copy(&parser->tmp_string, parser->token.tvalue.string);
        get_token();
        //copy param id into tmp_string
        token_ttype(20); // we found : after ID
        get_token();
        token_int_num_str(); // is it integer,number or string?
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        //push to stack [name:int/num/string]
        stack_push(&parser->tmp_stack, parser->tmp_string);
        parser->num_params++;
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
        string_copy(&parser->tmp_string, parser->token.tvalue.string);
        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_int_num_str(); // is it integer,number or string?
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, ':');
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        stack_push(&parser->tmp_stack, parser->tmp_string);
        parser->num_params++;
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
        string_clear(&parser->tmp_string);
        token_int_num_str(); // is it integer,number or string?
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        parser->num_return++;
        stack_push(&parser->tmp_stack_2, parser->tmp_string);
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
        string_clear(&parser->tmp_string);
        token_int_num_str(); // is it integer,number or string?
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        parser->num_return++;
        stack_push(&parser->tmp_stack_2, parser->tmp_string);
        parser->exit_code = return_type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of return_type_2()

int type(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_int_num_str(parser)) { //token is INT/NUM/STR
        string_clear(&parser->tmp_string);
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        stack_push(&parser->tmp_stack, parser->tmp_string);
        parser->num_types++;
        return type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of type()

int type_2(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_ttype(parser, 12)) { // ,
        get_token();
        token_int_num_str(); // is it integer,number or string?
        string_clear(&parser->tmp_string);
        if (is_kword(parser, 6)) {  //int
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
        } else if (is_kword(parser, 8)) {   //double
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
        } else if (is_kword(parser, 7)) {   //string
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
        }
        stack_push(&parser->tmp_stack, parser->tmp_string);
        parser->num_types++;
        parser->exit_code = type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of type_2()

int args(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_utype(parser, 3)) { // <value> -> ID
        return args_2(parser);
    } else if (is_ttype(parser, 13)) { // <value> -> INT
        return args_2(parser);
    } else if (is_ttype(parser, 37)) { // <value> -> NUMBER(double)
        return args_2(parser);
    } else if (is_ttype(parser, 29)) { // <value> -> STRING
        return args_2(parser);
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of args()

int args_2(Parser parser) {
    get_token();
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_ttype(parser, 12)) {// ,
        get_token();
        if (is_utype(parser, 3)) { // <value> -> ID
            parser->exit_code = args_2(parser);
        } else if (is_utype(parser, 13)) { // <value> -> INT
            parser->exit_code = args_2(parser);
        } else if (is_utype(parser, 37)) { // <value> -> NUMBER(double)
            parser->exit_code = args_2(parser);
        } else if (is_utype(parser, 29)) { // <value> -> STRING
            parser->exit_code = args_2(parser);
        } else {
            return PARSER_ERR;
        }
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of args_2()

int expression_if(Parser parser) {
    while(1) {
        get_token();
        //isID()
        //search-bintree -> ID value
        //push_stack
        if (is_kword(parser, 13)) {
            //parser->exit_code = eval(stack)
            // if (exit_code != 0) return exitcode
            break;
        }
    }
    return 0;
}

int expression_while(Parser parser) {
    while(1) {
        get_token();
        //isID()
        //search-bintree -> ID value
        //push_stack
        if (is_kword(parser, 15)) {
            //parser->exit_code = eval(stack)
            // if (exit_code != 0) return exitcode
            break;
        }
    }
    return 0;
}

int expression_return(Parser parser) {
    while(1) {
        get_token_EOL_gut();
        //isID()
        //search-bintree -> ID value
        //push_stack
        if (is_ttype(parser, 2)) {
            //parser->exit_code = eval(stack)
            // if (exit_code != 0) return exitcode
            break;
        }
    }
    return 0;
}

int assign(Parser parser) {
    get_token();
    token_int_num_str_nil(); 
    get_token_EOL_gut();
    if (is_ttype(parser, 2)) { // EOL
        return 0; 
    }
    if (is_ttype(parser, 41)) { // =
        get_token();
        if (is_utype(parser, 3)) { //ID
            get_token_EOL_gut();    // has to be EOL
            if (is_ttype(parser, 14)) {// (
                parser->exit_code = args(parser);
                if (parser->exit_code != 0) {
                    return parser->exit_code;
                }
                return 0;
            }
            if (is_ttype(parser, 2)) { // EOL
                return 0; 
            }
            return PARSER_ERR;
        } else if (is_ttype(parser, 13)) { //INT value
            get_token_EOL_gut();    // has to be EOL
            if (is_ttype(parser, 2)) { // EOL
                return 0; 
            }
            return PARSER_ERR;
        } else if (is_ttype(parser, 37)) { //NUMBER value
            get_token_EOL_gut();    // has to be EOL
            if (is_ttype(parser, 2)) { // EOL
                return 0; 
            }
            return PARSER_ERR;
        } else if (is_ttype(parser, 29)) { // STRING value 
            get_token_EOL_gut();    // has to be EOL
            if (is_ttype(parser, 2)) { // EOL
                return 0; 
            }
            return PARSER_ERR;
        } else if (is_kword(parser, 10)) { // NIL value
            get_token_EOL_gut();    // has to be EOL
            if (is_ttype(parser, 2)) { // EOL
                return 0; 
            }
            return PARSER_ERR;
        } else {
            //expression
            /*
            while(1) {
                get_token_EOL_gut();
                //isID()
                //search-bintree -> ID value
                //push_stack
                if (is_ttype(parser, 2)) {
                    //parser->exit_code = eval(stack)
                    // if (exit_code != 0) return exitcode
                    break;
                }
            }
            return 0;
            */
           return PARSER_ERR;
        }
    } else {
        return PARSER_ERR;
    }
    return 0;
}

int id(Parser parser) {
    get_token();
    token_isID();
    get_token();
    if (is_ttype(parser, 12)) {// ,
        parser->exit_code = id(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    } else if (is_ttype(parser, 41)){ // =
        //expression
            while(1) {
                get_token_EOL_gut();
                //isID()
                //search-bintree -> ID value
                //push_stack
                if (is_ttype(parser, 2)) {
                    //parser->exit_code = eval(stack)
                    // if (exit_code != 0) return exitcode
                    break;
                }
            }
            return 0;
    } else {
        return PARSER_ERR;
    }
    return 0;
}


//term = INT, DOUBLE(number), STRING, NIL, ID



int body(Parser parser) {
    get_token();
    if (is_kword(parser, 5)) {  //IF
        parser->exit_code = expression_if(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->_if++;
        parser->end = false;
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }


    } else if (is_kword(parser, 14)) {  //WHILE
        parser->exit_code = expression_while(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->_while++;
        parser->end = false;
        parser->wend = true;
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    
    
    
    } else if (is_kword(parser, 12)) {  //RETURN
        parser->exit_code = expression_return(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

    } else if (is_utype(parser, 3)) { // ID
        if (string_cmp(parser->token.tvalue.string, "write") == 0) {
            get_token();
            if (is_ttype(parser, 14)) {
                while(1) {
                //get_token();
                getNextToken(&parser->token);
                //isID()
                //search-bintree -> ID value
                //push_stack
                if (is_ttype(parser, 15)) {
                    //parser->exit_code = eval(stack)
                    // if (exit_code != 0) return exitcode
                    break;
                }
                }
            } else {
                return PARSER_ERR;
            }
            parser->exit_code = body(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }
            return 0;
        }
        get_token();
        if (is_ttype(parser, 14)) {   // (
            parser->exit_code = args(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }
        } else if (is_ttype(parser, 12)) { // ,
            parser->exit_code = id(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }
        } else if (is_ttype(parser, 41)) { // =
            //expression
            while(1) {
                get_token_EOL_gut();
                //isID()
                //search-bintree -> ID value
                //push_stack
                if (is_ttype(parser, 2)) {
                    //parser->exit_code = eval(stack)
                    // if (exit_code != 0) return exitcode
                    break;
                }
            }
            //return 0;
        } else {
            return PARSER_ERR;
        }
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }



    } else if (is_kword(parser, 9)) {   //LOCAL
        get_token();
        token_isID();   // is ID
        get_token();
        token_ttype(20); // is :
        parser->exit_code = assign(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }



    } else if (is_kword(parser, 1) && parser->_if != 0) {   //ELSE
        parser->_else++;

        if (parser->_if == parser->_else) {
            parser->wend = false;
        }
        parser->end = true;
        parser->exit_code = body(parser);
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    
    
    
    
    } else if (is_kword(parser, 2)) {   //END
        if (parser->_if == 0 && parser->_while == 0) {
            return 0;
        }

        else if (parser->_while != 0 && parser->wend == true) {
            parser->_while--;
            if (parser->_while == 0) {
                parser->wend = false;
                parser->end = true;
            }
            parser->exit_code = body(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }
        }

        else if (parser->_if != 0 && parser->end == true) {
            parser->_if--;
            parser->_else--;
            if (parser->_if == parser->_else) {
                parser->wend = true;
                parser->end = true;
            }
            parser->exit_code = body(parser);
            if (parser->exit_code != 0) {
                return parser->exit_code;
            }

        }

        else {
            return PARSER_ERR;
        }
    } else {
        return PARSER_ERR;
    }
    return 0;
}
