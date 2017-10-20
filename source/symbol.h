/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Implements the symbol table
 * Due: 10/26/2017
 */

#include "global.h"

struct entry {
    struct entry* next;
    char lexeme[STRMAX];
    int tokenType;
};

int getType(char* lexeme);
int hash (char s[]);
int lookup (char s[]);
int insert (char s[], int tok);
struct entry* getSym(int i);
int init();