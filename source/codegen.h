/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Generates register based code
 * Due: 10/26/2017
 */

#include "global.h"

void openFile(char s[]);
void program(struct pnode *parent);
void statements(struct pnode* root);
void declaration(struct pnode *parent);
void assignment();
char* expression();
void iterator();
void selector();