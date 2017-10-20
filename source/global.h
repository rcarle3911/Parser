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
#include <string.h>

#define STRMAX 100
#define SYMMAX 100
#define BSIZE 100
#define NONE -1
#define EOS '\0'
#define MAIN 256
#define INT 257
#define ADDSUBOP 258
#define CONOP 259
#define TYPE 260
#define WHILE 261
#define IF 262
#define ELSE 263
#define ID 264
#define DONE 265
#define DECLARATION 266
#define ASSIGNMENT 267
#define REGISTER 268
#define MULTDIVOP 269
#define READ 270
#define WRITE 271

int lineno;
char lexbuf[BSIZE];
