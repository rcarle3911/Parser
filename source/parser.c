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
#include "parser.h"
#include "lexer.h"
#include "error.h"
#include "symbol.h"

int lookahead;

struct pnode* newPNode() {
    struct pnode *node = malloc(sizeof(*node));
    node->left = NULL;
    node->right = NULL;
    node->type = NONE;
    strcpy(node->value, "");
    return node;
}

struct pnode* parse() {
    lookahead = lexan();
    struct pnode *prgrm = newPNode();
    prgrm->type = lookahead;
    strcpy(prgrm->value, lexbuf);
    match(MAIN);
    prgrm->left = statementblock();

    if (lookahead!= DONE) error("Program ended unexpectedly");
    printf("Program is valid\n\n");
    return prgrm;
}

struct pnode* statementblock() {
    match('{');
    struct pnode *sblock = newPNode();
    struct pnode *temp;
    struct pnode *root = sblock;
    while (lookahead != '}' && lookahead != DONE) {
        strcpy(sblock->value, "SBLOCK");
        sblock->left = statement();
        if (lookahead != '}' && lookahead != DONE) {
            temp = sblock;
            sblock = newPNode();
            temp->right = sblock;
        }
    }
    match('}');
    return root;
}

struct pnode* statement() {
    switch (lookahead){
        case TYPE:
            return decl();
        case ID:
            return assg();
        case WHILE:
            return iter();
        case IF:
            return sel();
        case READ:
        case WRITE:
            return rw();
        default:
            error("Expected a statement");
    }
}

struct pnode* rw() {
    struct pnode *rewr = newPNode();
    rewr->type = lookahead;
    match(lookahead);
    match('(');
    if (!lookup(lexbuf)) {
        char err[BSIZE + 40];
        sprintf(err, "Variable %s has not been declared", lexbuf);
    }
    strcpy(rewr->value, lexbuf);
    match(ID);
    match(')');
    match(';');
    return rewr;
}

struct pnode* decl() {
    struct pnode *dec = newPNode();
    struct pnode *dec_left = newPNode();
    struct pnode *type = newPNode();
    struct pnode *id = newPNode();
    dec->type = DECLARATION;
    dec->left = dec_left;
    dec_left->left = type;
    dec_left->right = id;

    type->type = lookahead;
    strcpy(type->value, lexbuf);
    match(TYPE);

    if (lookup(lexbuf)) {
        char err[BSIZE + 40];
        sprintf(err, "Variable %s is already declared", lexbuf);
        error(err);
    }
    insert(lexbuf, lookahead);
    id->type = lookahead;
    strcpy(id->value, lexbuf);
    match(ID);

    if (lookahead != ';') {
        match('=');
        dec->right = expr();
    }
    match(';');
    return dec;
}

struct pnode* assg() {
    if (!lookup(lexbuf)) {
        char err[BSIZE + 40];
        sprintf(err, "Variable %s has not been declared", lexbuf);
    }
    struct pnode* asg = newPNode();
    struct pnode* asg_left = newPNode();
    asg->type = ASSIGNMENT;
    asg->left = asg_left;
    asg_left->type = lookahead;
    strcpy(asg_left->value, lexbuf);
    match(ID);
    match('=');
    asg->right = expr();
    match(';');
    return asg;
}

struct pnode* expr() {
    struct pnode *exp = newPNode();
    switch (lookahead) {
        case '(':
            match('(');
            exp->left = expr();
            match(')');
            break;
        case ADDSUBOP:
            strcpy(exp->value, lexbuf);
            match(lookahead);
        case INT:
            strcat(exp->value, lexbuf);
            exp->type = lookahead;
            match(INT);
            break;
        case ID:
            exp->type = lookahead;
            strcpy(exp->value, lexbuf);
            match(ID);
            break;
        default:
            error("Expected an operand");
    }
    exp->right = oper();
    return exp;
}

// Having an operator is optional, but if it's there it must be followed by an expression.
struct pnode* oper() {
    struct pnode *op;
    switch (lookahead) {
        case MULTDIVOP:
        case ADDSUBOP:
            op = newPNode();
            op->type = lookahead;
            strcpy(op->value, lexbuf);
            match(lookahead);
            op->right = expr();
            return op;
        default:
            return NULL;
    }
}

struct pnode* cond() {
    struct pnode* con = newPNode();
    match('(');
    con->left = expr();
    con->type = lookahead;
    strcpy(con->value, lexbuf);
    match(CONOP);
    con->right = expr();
    match(')');
    return con;
}

struct pnode* iter() {
    struct pnode* iterator = newPNode();
    iterator->type = lookahead;
    strcpy(iterator->value, lexbuf);
    match(WHILE);
    iterator->left = cond();
    iterator->right = statementblock();
    return iterator;
}

struct pnode* sel() {
    struct pnode* selector = newPNode();
    struct pnode* branch = newPNode();
    selector->type = lookahead;
    strcpy(selector->value, lexbuf);
    match(IF);
    selector->left = cond();
    selector->right = branch;
    strcpy(branch->value, "BRANCH");
    branch->left = statementblock();
    if (lookahead== ELSE) {
        match(ELSE);
        branch->right = statementblock();
    }
    return selector;
}

void match(int token) {
    if (lookahead == token) {
        lookahead = lexan();
    }
    else
        error("Syntax error");
}
