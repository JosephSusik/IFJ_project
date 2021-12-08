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
    parser->num_args = 0;
    stack_init(&parser->tmp_stack);
    stack_init(&parser->tmp_stack_2);
    stack_init(&parser->tmp_stack_3);
    string_init(&parser->tmp_string);
    return parser;
}

int free_parser(Parser parser) {
    symtable_dispose(&parser->global_symtable);
    string_free(&parser->func_id);
    stack_dispose(&parser->tmp_stack);
    //stack_dispose(&parser->tmp_stack_2);
    //stack_dispose(&parser->tmp_stack_3);
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
    printf("\n");

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
        string_clear(&parser->tmp_string);
        /*
        string_add_char(&parser->tmp_string, 'f');
        string_add_char(&parser->tmp_string, 'r');
        string_add_char(&parser->tmp_string, 'a');
        string_add_char(&parser->tmp_string, 'n');
        string_add_char(&parser->tmp_string, 't');
        string_add_char(&parser->tmp_string, 'a');
        */
        /*
        string_add_char(&parser->tmp_string, 'i');
        string_add_char(&parser->tmp_string, 'v');
        string_add_char(&parser->tmp_string, 'o');
        string_add_char(&parser->tmp_string, 's');
        string_add_char(&parser->tmp_string, '\0');
        */
        String *pointer = parser->token.tvalue.string;
        nodeptr a = symtable_search(&parser->global_symtable, pointer);
        //printf("pozice ivos %p\n", &a);

        string_copy(&parser->func_id, parser->token.tvalue.string);

        //string_print(pointer);
        //printf("\n");
        
        if(a != NULL) {
            //string_print(a->key);
            if (a->func_data->defined == true) {
                //printf("tu\n");
                return UNDEFINED_VAR_ERR;
            }
        }
        stack nul;
        stack_init(&nul);
    
        //stack_push(&parser->params_stack, parser->token.tvalue.string + type);
        get_token();
        token_ttype(14); // we found ( after ID

        stack_dispose(&parser->tmp_stack);
        parser->num_params = 0;
        parser->exit_code = params(parser); //process <params>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

        /*
        -load return params into stack
        */
        stack_dispose(&parser->tmp_stack_2);
        parser->num_return = 0;
        parser->exit_code = return_type(parser); //process <return_type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        if(a != NULL) {
            if (a->func_data->declared == true) {
                //printf("%i %i", a->func_data->num_params, parser->num_params);
                if (a->func_data->num_params == parser->num_params) {

                    while(parser->tmp_stack.top != NULL) {
                        stack_push(&parser->tmp_stack_3, parser->tmp_stack.top->data);
                        stack_pop(&parser->tmp_stack);
                    }

                    stackptr *point_2 = parser->tmp_stack_3.top;
                    stackptr *point = a->func_data->stack_params.top;     

                    //params
                    for(int i = 0; i < parser->num_params; i++) {
                        //string_print(&point->data);
                        //printf("    ");
                        //string_print(&point_2->data);
                        //printf("\n");
                        string_strtok(&point_2->data, ":", &parser->tmp_string);
                        
                        if(string_string_cmp(&point->data, &parser->tmp_string) != 0){
                            
                            //This is the end
                            //printf("tu1\n");
                            parser->exit_code = UNDEFINED_VAR_ERR;
                            return parser->exit_code;
                            
                        }
                        
                        string_clear(&parser->tmp_string);
                        point = point->next;
                        point_2 = point_2->next;

                    } 
                    
                    stack_dispose(&(parser->tmp_stack));
                   
                } else {
                    //printf("tu2\n");
                    parser->exit_code = UNDEFINED_VAR_ERR;
                    return parser->exit_code;
                }
                if (a->func_data->num_return == parser->num_return) {
                    //printf("adresa %p\n", &parser->tmp_stack_2.top->data);
                    while(parser->tmp_stack_2.top != NULL) {
                        stack_push(&parser->tmp_stack, parser->tmp_stack_2.top->data);
                        stack_pop(&parser->tmp_stack_2);
                        //printf("adresa %p\n", &parser->tmp_stack_2.top->data);
                    }
                    //printf("adresa %p\n", &parser->tmp_stack_2.top->data);
                    
                    
                    //stack_dispose(&(parser->tmp_stack_2));
                    

                    stackptr *point_2 = parser->tmp_stack.top;
                    stackptr *point = a->func_data->stack_returns.top;  
                    
                    //printf("--returns---\n");
                    
                    //returns
                    for(int i = 0; i < parser->num_return; i++) {
                        //string_print(&point->data);
                        //printf("    ");
                        //string_print(&point_2->data);
                        //printf("\n");
                        
                        if(string_string_cmp(&point->data, &point_2->data) != 0){
                            
                            //This is the end
                            printf("tu3\n");
                            parser->exit_code = UNDEFINED_VAR_ERR;
                            return parser->exit_code;
                            
                        }
                        point = point->next;
                        point_2 = point_2->next;
                    }                   
                } else {
                    printf("tu4\n");
                    parser->exit_code = UNDEFINED_VAR_ERR;
                    return parser->exit_code;
                }
            }
         
            //returns, jsou ve _2, z 1 
            while(parser->tmp_stack.top != NULL) {
                stack_push(&parser->tmp_stack_2, parser->tmp_stack.top->data);
                stack_pop(&parser->tmp_stack);
            }

            //params, budou v _1 (z 3 do 1)
            while(parser->tmp_stack_3.top != NULL) {
                stack_push(&parser->tmp_stack, parser->tmp_stack_3.top->data);
                stack_pop(&parser->tmp_stack_3);
            }

        }


        //printf("insert\t");
        //string_print(&parser->func_id);
        //printf("\n");

        String new_string;
        String *p_new_string = &new_string;
        string_init(p_new_string);
        //string_copy(p_new_string,parser->token.tvalue.string);
        string_copy(p_new_string, &parser->func_id);

        symtable_insert(&parser->global_symtable, &new_string, func, true, true, parser->num_params, parser->num_return, parser->tmp_stack, parser->tmp_stack_2);
        /*
        nodeptr b = symtable_search(&parser->global_symtable, &parser->func_id);
        string_print(&b->func_data->stack_params.top->data);
        string_print(&b->func_data->stack_params.top->next->data);
        string_print(&b->func_data->stack_params.top->next->next->data);
        string_print(&b->func_data->stack_params.top->next->next->next->data);
        
        string_print(&b->func_data->stack_returns.top->data);
        string_print(&b->func_data->stack_returns.top->next->data);
        */

        stack_dispose(&(parser->tmp_stack));

        //string_print(&parser->tmp_stack.top->data);

        //THIS IS HOW U ACCESS NODE WITH NAME
        //nodeptr b = symtable_search(&parser->global_symtable, &parser->func_id);
      
        nodeptr b = symtable_search(&parser->global_symtable, &parser->func_id);

        printf("LABEL $");
        string_print(b->key);
        printf("\n");
        printf("PUSHFRAME\n");
        if (b->func_data->num_return != 0) {
            for (int i = 1; i <= b->func_data->num_return; i++) {
                printf("DEFVAR LF@%%retval%i\n", i);
                printf("MOVE LF@%%retval%i nil@nil\n", i);
            }
        }

        if (b->func_data->num_params != 0) {
            for (int i = 1; i <= b->func_data->num_params; i++) {
                printf("DEFVAR LF@%%param%i\n", i);
                printf("MOVE LF@%%param%i LF@%%%d\n", i, i);
            }
        }



        parser->exit_code = body(parser); //process <body>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        //printf("RUN AGAIN\n");
        //string_print(&parser->func_id);
        //printf("\n");

        printf("POPFRAME\n");
        printf("RETURN\n");
        printf("\n");

        parser->exit_code = prog(parser); //process <prog>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }

    //GLOBAL
    } else if (is_kword(parser, 4)) {
        get_token();
        token_isID(); // we found ID after GLOBAL

        string_clear(&parser->func_id);

        string_copy(&parser->func_id,parser->token.tvalue.string);

        String *pointer = parser->token.tvalue.string;
        nodeptr a = symtable_search(&parser->global_symtable, pointer);
        
        //nodeptr a = symtable_search(&parser->global_symtable, &parser->func_id);
        
        if(a != NULL) {
            if (a->func_data->declared == true) {
                printf("tu6\n");
                return UNDEFINED_VAR_ERR;
            }
        }
        //string_copy(&parser->func_id,parser->token.tvalue.string);
        
        get_token();
        token_ttype(20); // we found : after ID
        get_token();
        token_kword(3); // we found FUNCTION after :
        get_token();
        token_ttype(14); // we found ( after ID

        stack_dispose(&(parser->tmp_stack_3));
        parser->num_params = 0;
        parser->num_types = 0;
        parser->exit_code = type(parser); //process <type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        stack_dispose(&parser->tmp_stack_2);
        parser->num_return = 0;
        parser->exit_code = return_type(parser); //process <return_type>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        /*
        string_print(&parser->tmp_stack_3.top->data);
        string_print(&parser->tmp_stack_3.top->next->data);
        string_print(&parser->tmp_stack_3.top->next->next->data);
        string_print(&parser->tmp_stack_3.top->next->next->next->data);*/

        //printf("==>> %s\n", parser->func_id.str);
        
        String new_string;
        String *p_new_string = &new_string;
        string_init(p_new_string);
        //string_copy(p_new_string,parser->token.tvalue.string);
        string_copy(p_new_string, &parser->func_id);

        /*
        String new_string;
        string_init(&new_string);
        string_copy(&new_string, &parser->func_id);
        */

        symtable_insert(&parser->global_symtable, &new_string, func, true, false, parser->num_types, parser->num_return, parser->tmp_stack_3, parser->tmp_stack_2);
        //nodeptr b = symtable_search(&parser->global_symtable, &parser->func_id);
        //string_print(&b->func_data->stack_params.top->next->next->next->data);
    
        stack_dispose(&(parser->tmp_stack_3));
        stack_dispose(&(parser->tmp_stack_2));

        //string_print(&parser->tmp_stack_3.top->data);

        parser->exit_code = prog(parser); //process <prog>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
    
    //ID
    } else if (is_utype(parser, 3)) {
        //maybe with bool??
        //prints LABEL $$main
        print_main();

        string_copy(&parser->func_id ,parser->token.tvalue.string);
        //nodeptr a = symtable_search(&parser->global_symtable, parser->token.tvalue.string);

        /*
        if(a != NULL){
            printf("Nasel jsem funkci\n");
        }
        else{
            printf("Nenasel jsem funkci\n");
        }
        */

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
        parser->num_args = 0;

        stack_dispose(&parser->tmp_stack_2);

        parser->exit_code = args(parser); //process <args>
        if (parser->exit_code != 0) {
            return parser->exit_code;
        }
        
        /*
        Check it function is in tree and if its call with right number of params
        String *pointer = &parser->func_id;
        nodeptr a = symtable_search(&parser->global_symtable, pointer);
        */

        String *pointer = &parser->func_id;
        nodeptr a = symtable_search(&parser->global_symtable, pointer);

        while (parser->tmp_stack.top != NULL) {
            stack_push(&parser->tmp_stack_3, parser->tmp_stack.top->data);
            stack_pop(&parser->tmp_stack);
        }


        if(a != NULL){
            //printf("Nasel jsem funkci : ");
            //string_print(a->key);
            //printf("\n");
            if (a->func_data->num_params == parser->num_args) {
                
                stackptr *point_2 = parser->tmp_stack_3.top;
                stackptr *point = a->func_data->stack_params.top;
                
                //string_clear(&parser->tmp_string);
                
                for (int i = 0; i < parser->num_args; i++) {
                        //string_print(&point->data);
                        //printf("    ");
                        //string_print(&point_2->data);
                        //printf("\n");
                        
                        string_strtok(&point->data, ":", &parser->tmp_string);
                                                
                        if(string_string_cmp(&point_2->data, &parser->tmp_string) != 0){
                            //if argument is type num and is passed int, still should work
                            if (string_cmp(&parser->tmp_string, "num") == 0) {
                                if (string_cmp(&point_2->data, "int") != 0) {
                                    //printf("int num\n");
                                    //printf("tu1\n");
                                    parser->exit_code = UNDEFINED_VAR_ERR;
                                    return parser->exit_code;
                                } else {
                                    //printf("to Number inserted INT\n");
                                }
                            } else {
                                //printf("tu1\n");
                                parser->exit_code = UNDEFINED_VAR_ERR;
                                return parser->exit_code;
                            }
                            /*
                            //This is the end
                            printf("tu1\n");
                            parser->exit_code = UNDEFINED_VAR_ERR;
                            return parser->exit_code;
                            */
                        }
                        
                        
                        string_clear(&parser->tmp_string);
                        point = point->next;
                        point_2 = point_2->next;

                }
                
                
                
                //printf("Ma spravny pocet argumentu\n");
            } else {
                //printf("Ma SPATNY pocet argumentu\n");
                parser->exit_code = PARAM_ERR;
                return parser->exit_code;
            }
        } else{
            //printf("Nenasel jsem funkci\n");
            parser->exit_code = UNDEFINED_VAR_ERR;
            return parser->exit_code;
        }
        
        //PRINT 3 ADRESS CODE FOR THE CALL OF FUNCTION
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");
        printf("CREATEFRAME\n");
        for(int i = 1; i <= parser->num_args; i++) {
            printf("DEFVAR TF@%%%d\n", i);
        }        
        
        //move values into the TF@
        //when reading them, read them into stack like type@value
        // then print them 

        stack_dispose(&parser->tmp_stack);

        for(int i = 1; i <= parser->num_args; i++) {
            printf("MOVE TF@%%%d something\n", i);
        }     

        //call the function
        
        printf("CALL $"); 
        string_print(a->key);
        printf("\n");
        

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
        string_clear(&parser->tmp_string);
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
        string_clear(&parser->tmp_string);
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
        stack_push(&parser->tmp_stack_3, parser->tmp_string);
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
        stack_push(&parser->tmp_stack_3, parser->tmp_string);
        parser->num_types++;
        parser->exit_code = type_2(parser);
    } else {
        return PARSER_ERR;
    }
    return parser->exit_code;
}//end of type_2()

int args(Parser parser) {
    get_token();
    string_clear(&parser->tmp_string);
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_utype(parser, 3)) { // <value> -> ID
        parser->num_args++;
        return args_2(parser);
    } else if (is_ttype(parser, 13)) { // <value> -> INT
        string_add_char(&parser->tmp_string, 'i');
        string_add_char(&parser->tmp_string, 'n');
        string_add_char(&parser->tmp_string, 't');
        stack_push(&parser->tmp_stack, parser->tmp_string);
        string_clear(&parser->tmp_string);
        sprintf(parser->tmp_string.str, "%d", parser->token.tvalue.whole_num);

        printf("tu\n");
        string_print(&parser->tmp_string);
        printf("\n");

        stack_push(&parser->tmp_stack_2, parser->tmp_string);
        parser->num_args++;
        return args_2(parser);
    } else if (is_ttype(parser, 37)) { // <value> -> NUMBER(double)
        string_add_char(&parser->tmp_string, 'n');
        string_add_char(&parser->tmp_string, 'u');
        string_add_char(&parser->tmp_string, 'm');
        stack_push(&parser->tmp_stack, parser->tmp_string);
        string_clear(&parser->tmp_string);
        sprintf(parser->tmp_string.str, "%f", parser->token.tvalue.dec_num);
        stack_push(&parser->tmp_stack_2, parser->tmp_string);
        parser->num_args++;
        return args_2(parser);
    } else if (is_utype(parser, 5)) { // <value> -> STRING  
        string_add_char(&parser->tmp_string, 's');
        string_add_char(&parser->tmp_string, 't');
        string_add_char(&parser->tmp_string, 'r');
        stack_push(&parser->tmp_stack, parser->tmp_string);
        //stack_push(&parser->tmp_stack_2, parser->token.tvalue.string);
        parser->num_args++;
        return args_2(parser);
    } else {
        return PARSER_ERR;
    }
    return 0;
}//end of args()

int args_2(Parser parser) {
    get_token();
    string_clear(&parser->tmp_string);
    if (is_ttype(parser, 15)) { //token is )
        return 0;
    } else if (is_ttype(parser, 12)) {// ,
        get_token();
        if (is_utype(parser, 3)) { // <value> -> ID
            parser->num_args++;
            parser->exit_code = args_2(parser);
        } else if (is_ttype(parser, 13)) { // <value> -> INT
            string_add_char(&parser->tmp_string, 'i');
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 't');
            stack_push(&parser->tmp_stack, parser->tmp_string);
            parser->num_args++;
            parser->exit_code = args_2(parser);
        } else if (is_ttype(parser, 37)) { // <value> -> NUMBER(double)
            string_add_char(&parser->tmp_string, 'n');
            string_add_char(&parser->tmp_string, 'u');
            string_add_char(&parser->tmp_string, 'm');
            stack_push(&parser->tmp_stack, parser->tmp_string);
            parser->num_args++;
            parser->exit_code = args_2(parser);
        } else if (is_utype(parser, 5)) { // <value> -> STRING
            string_add_char(&parser->tmp_string, 's');
            string_add_char(&parser->tmp_string, 't');
            string_add_char(&parser->tmp_string, 'r');
            stack_push(&parser->tmp_stack, parser->tmp_string);
            parser->num_args++;
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
            if (is_ttype(parser, 14)) {     // (
                while(1) {
                //get_token();
                getNextToken(&parser->token);
                if (is_utype(parser, 5)) {  // its STRING
                    printf("WRITE string@");
                    //string_print(parser->token.tvalue.string);
                    for(int i = 0; i < parser->token.tvalue.string->length; i++) {
                        if (parser->token.tvalue.string->str[i] < 33 || parser->token.tvalue.string->str[i] == 35 || parser->token.tvalue.string->str[i] == 92) {
                            int a = parser->token.tvalue.string->str[i];
                            printf("/0%i",a);
                        } else {
                            printf("%c", parser->token.tvalue.string->str[i]);

                        }
                    }
                    printf("\n");
                }
                if (is_utype(parser, 3)) {  // its ID
                    nodeptr a = symtable_search(&parser->global_symtable, &parser->func_id);
                    if (a != NULL) {
                        bool par_f = false;
                        stackptr *point = a->func_data->stack_params.top;
                        for (int i = 1; i <= a->func_data->num_params; i++) {
                            string_clear(&parser->tmp_string);
                            string_copy(&parser->tmp_string, &point->data);
                            int len = strlen(parser->tmp_string.str);
                            parser->tmp_string.str[len-4] = '\0';
                            parser->tmp_string.str[len-3] = '\0';
                            parser->tmp_string.str[len-2] = '\0';
                            parser->tmp_string.str[len-1] = '\0';
                        
                            if (string_string_cmp(&parser->tmp_string, parser->token.tvalue.string) == 0) {
                                printf("WRITE LF@param%d\n", i);
                                par_f = true;
                            }
                            point = point->next;
                        }
                        if (par_f == false) {
                            //printf("not found\n");
                            parser->exit_code = UNDEFINED_VAR_ERR;
                            return parser->exit_code;
                        }
                    } else {
                        parser->exit_code = UNDEFINED_VAR_ERR;
                        return parser->exit_code;
                    }
                }
       





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
