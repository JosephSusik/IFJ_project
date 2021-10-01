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
* @file compilerIFJ21.c
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "parser.h"


int reading_file(int *filename){
    FILE *infile;
    char *buffer;
    long numbytes;

    /* open an existing file for reading */
    infile = fopen(filename, "r");
 
    /* quit if the file does not exist */
    if(infile == NULL)
        exit(INTERNAL_ERR);

    /* Get the number of bytes */
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
 
    /* reset the file position indicator to 
    the beginning of the file */
    fseek(infile, 0L, SEEK_SET);	
 
    /* grab sufficient memory for the 
    buffer to hold the text */
    buffer = (char*)calloc(numbytes, sizeof(char));	
 
    /* memory error */
    if(buffer == NULL){
        free(buffer);
        exit(INTERNAL_ERR);
    }
 
    /* copy all the text into the buffer */
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);
 
    char text[numbytes];
    strcpy(text, buffer);
    
    /* free the memory we used for the buffer */
    free(buffer);
    parser(&text);

    return OK;
}

int main(int argc, char **argv) {
    if (argc != 2){
        printf("Program musi byt spusten s jednim argumentem !!\n");
        return INTERNAL_ERR;
    }
    int *filename = argv[1];
    reading_file(filename);


    return OK;
}