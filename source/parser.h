/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Parser
 * Due: 10/26/2017
 */

#include "global.h"
#include "lexer.h"
#include "error.h"

void parse();
void statementblock();
void statement();
void decl();
void assg();
void expr();
void oper();
void iter();
void sel();
void condition();
void match(int t);