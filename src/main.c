/***************************************
* Project - Compiler for IFJ21
*
* @brief Main file
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author Samuel Popelář <xpopel22>
*
* @file main.c
**************************************/

#include <stdio.h>
#include "scanner.h"
#include "parser.h"


int main(int argc, char* argv[]) {
	
	FILE *f;
	/*
	if (argc == 1) {
		printf("Není zadán vstupní soubor!\n");
		return 99;
	}
	*/
	if ((f= fopen(argv[1], "r")) == NULL) {
		//printf("Soubor se nepodarilo otevrit\n");
      	//return 99;
		f = stdin;
	}


	setSourceFile(f);
	String str;
    if (string_init(&str) == 1)
    {
        return INTERNAL_ERR;
    }
    setString(&str);
	
	int exit = parse();
	if (exit != 0) {
		printf("Something went wrong\n");
		return exit;
	}
	//printf("Hello IFJ\n");

	return 0;
}
