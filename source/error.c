/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Error handler
 * Due: 10/5/2017
 */

#include "global.h"
#include "error.h"
#include "lexer.h"

int error(char *m)
{
    fprintf(stderr, "Program is invalid \nline %d: %s\n\n", lineno, m);
    freeMemory();
    exit(1);
}