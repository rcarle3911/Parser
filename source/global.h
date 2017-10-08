/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Global declarations
 * Due: 10/5/2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define STRMAX 100
#define SYMMAX 100
#define BSIZE 128
#define NONE -1
#define EOS '\0'
#define MAIN 256
#define INT 257
#define CONOP 259
#define TYPE 260
#define WHILE 261
#define IF 262
#define ELSE 263
#define ID 264
#define DONE 265

int tokenval;
int lineno;