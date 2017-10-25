/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Error handler
 * Due: 10/26/2017
 */

#include "error.h"
#include "lexer.h"
#include "parser.h"

int error(char *m)
{
    fprintf(stderr, "Program is invalid \nline %d: %s\n\n", lineno, m);
    freeMemory();
    freeParseTree();
    exit(1);
}