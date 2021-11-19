/***************************************
* Project - Compiler for IFJ21
*
* @brief Main file
*
* @author Josef Susík <xsusik00>
* @author Marek Putala <xputal00>
* @author
* @author
*
* @file main.c
**************************************/

#include <stdio.h>
#include "scanner.h"


int main(int argc, char* argv[]) {

	FILE *f;
	if (argc == 1) {
		printf("Není zadán vstupní soubor!\n");
		return 99;
	}

	if ((f= fopen(argv[1], "r")) == NULL) {
		printf("Soubor se nepodarilo otevrit\n");
      	return 99;
	}


	setSourceFile(f);

	parse();
	//printf("Hello IFJ\n");

	return 0;
}
