/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Implements the symbol table
 * Due: 10/5/2017
 */

#include "global.h"
#include "error.h"

struct entry {
    struct entry* next;
    char lexeme[STRMAX];
    int tokenType;
};

int hash (char s[]);
int lookup (char s[]);
int insert (char s[], int tok);
struct entry* getSym(int i);
int init();