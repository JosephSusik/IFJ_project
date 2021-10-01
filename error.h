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
* @file error.h 
**************************************/

#ifndef ERROR_H
#define ERROR_H

	#define OK 0
		
	#define SCANNER_ERR 1	// Faulty lexem of IFJ21
	#define PARSER_ERR 2	// Faulty syntax of IFJ21
	
	#define UNDEFINED_VAR_ERR 3	// Undefined function/var etc.
	#define DATA_TYPE_ERR 4	// Error while declaring data type
	#define ARITHMETICS_ERR 5	// Error in arithmetics etc.
	#define PARAM_ERR 6	// Wrong number of paramters or return types
	#define OTHER_ERR 7	// Other semantic errors

	#define DIV_ZERO_ERR 9	// Division by zero
	#define INTERNAL_ERR 99	// Error in the program, malloc etc.

#endif // ERROR_H