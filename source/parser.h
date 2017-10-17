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

struct pnode {
    struct pnode* parent;
    struct pnode* left;
    struct pnode* right;
    char value[STRMAX];
    int type;
};

void parse();
struct pnode* statementblock(struct pnode *parent);
struct pnode* statement(struct pnode *parent);
struct pnode* decl(struct pnode *parent);
struct pnode* assg(struct pnode *parent);
struct pnode* expr(struct pnode* parent);
struct pnode* oper(struct pnode* parent);
struct pnode* iter(struct pnode *parent);
struct pnode* sel(struct pnode *parent);
struct pnode* condition(struct pnode *parent);
void match(int t);