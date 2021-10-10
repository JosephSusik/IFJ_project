/***************************************
* Project - Compiler for IFJ21
* 
* @brief Compiler for IFJ21
* 
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author 
* @author
* 
* @file main.c 
**************************************/

#include <stdio.h>
#include "error.h"

int main(int argc, char *argv[]) {

    if (argc > 1){
        printf("Program nemuze but volan s vice jak jednim parametrem.\n");
        return INTERNAL_ERR;
    }

    //if not here, compiler errors, delete later
    printf("%s\n", argv[0]);

    
    
    return 0;
}

//End of main.c