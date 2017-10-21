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

struct pnode {
    struct pnode* left;
    struct pnode* right;
    char value[STRMAX];
    int type;
};

struct pnode* newPNode();
struct pnode* parse();
struct pnode* statementblock();
struct pnode* statement();
struct pnode* rw();
struct pnode* decl();
struct pnode* assg();
struct pnode* expr();
struct pnode* oper();
struct pnode* iter();
struct pnode* sel();
struct pnode* cond();
void freeParseTree(struct pnode *root);
void match(int t);