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
#include "codegen.h"

struct token* lookahead;
struct pnode prgrm = {NULL, NULL, NULL};

void parse() {
    lookahead = lexan();
    match(MAIN);
    prgrm.type = MAIN;
    prgrm.left = statementblock(&prgrm);
    prgrm.right = NULL;
    if (lookahead->type != DONE) error("Unexpected end of file");
    printf("Program is valid\n\n");
    program(&prgrm);
}

struct pnode* statementblock(struct pnode *parent) {
    match('{');
    struct pnode *sblock = malloc(sizeof(*sblock));
    struct pnode *root = sblock;
    sblock->parent = parent;
    while (lookahead->type != '}' && lookahead->type != DONE) {
        sblock->left = statement(sblock);
        if (lookahead->type == '}' || lookahead->type == DONE) sblock->right = NULL;
        else {
            parent = sblock;
            sblock = malloc(sizeof(*sblock));
            parent->right = sblock;
        }
    }
    match('}');
    return root;
}

struct pnode* statement(struct pnode *parent) {
    switch (lookahead->type) {
        case TYPE:
            return decl(parent);
        case ID:
            return assg(parent);
        case WHILE:
            return iter(parent);
        case IF:
            return sel(parent);
        default:
            error("Expected a statement");
    }
}

struct pnode* decl(struct pnode *parent) {
    struct pnode *dec = malloc(sizeof(*dec));
    struct pnode *dec_left = malloc(sizeof(*dec));
    struct pnode *type = malloc(sizeof(*dec));
    struct pnode *id = malloc(sizeof(*dec));
    dec->parent = parent;
    dec->type = DECLARATION;
    dec->left = dec_left;
    dec->right = NULL;
    dec_left->left = type;
    dec_left->right = id;

    type->type = lookahead->type;
    strcpy(type->value, lookahead->value);
    match(TYPE);

    id->type = lookahead->type;
    strcpy(id->value, lookahead->value);
    match(ID);

    if (lookahead->type != ';') {
        match('=');
        dec->right = expr(dec);
    }
    match(';');
    return dec;
}

struct pnode* assg(struct pnode* parent) {
    struct pnode* asg = malloc(sizeof(*asg));
    struct pnode* asg_left = malloc(sizeof(*asg));
    asg->parent = parent;
    asg->type = ASSIGNMENT;
    asg->left = asg_left;
    asg_left->parent = asg;
    asg_left->type = lookahead->type;
    strcpy(asg_left->value, lookahead->value);
    match(ID);
    match('=');
    asg->right = expr(asg);
    match(';');
}

struct pnode* expr(struct pnode* parent) {
    struct pnode *exp = malloc(sizeof(*exp));
    exp->parent = parent;
    exp->left = NULL;
    exp->right = NULL;
    switch (lookahead->type) {
        case '(':
            match('(');
            exp->left = expr(exp);
            match(')');
            break;
        case ADDSUBOP:
            strcpy(exp->value, lookahead->value);
            match(lookahead->type);
        case INT:
            strcat(exp->value, lookahead->value);
            exp->type = lookahead->type;
            match(INT);
            break;
        case ID:
            exp->type = lookahead->type;
            strcpy(exp->value, lexbuf);
            match(ID);
            break;
        default:
            error("Expected an operand");
    }
    exp->right = oper(exp);
    return exp;
}

// Having an operator is optional, but if it's there it must be followed by an expression.
struct pnode* oper(struct pnode* parent) {
    struct pnode *op;
    switch (lookahead->type) {
        case MULTDIVOP:
        case ADDSUBOP:
            op = malloc(sizeof(*op));
            op->parent = parent;
            op->left = NULL;
            op->type = lookahead->type;
            strcpy(op->value, lookahead->value);
            match(lookahead->type);
            op->right = expr(op);
            return op;
        default:
            return NULL;
    }
}

struct pnode* condition(struct pnode* parent) {
    match('(');
    expr(NULL);
    match(CONOP);
    expr(NULL);
    match(')');
}

struct pnode* iter(struct pnode* parent) {
    struct pnode* iterator = malloc(sizeof(*iterator));
    iterator->type = lookahead->type;
    strcpy(iterator->value, lookahead->value);
    match(WHILE);
    iterator->left = condition(iterator);
    iterator->right = statementblock(iterator);
}

struct pnode* sel(struct pnode* parent) {
    struct pnode* selector = malloc(sizeof(*selector));
    struct pnode* branch = malloc(sizeof(*branch));
    selector->type = lookahead->type;
    strcpy(selector->value, lookahead->value);
    match(IF);
    selector->left = condition(selector);
    selector->right = branch;
    branch->left = statementblock(branch);
    if (lookahead->type == ELSE) {
        match(ELSE);
        branch->right = statementblock(branch);
    }
}

void match(int token) {
    if (lookahead->type == token) {
        free(lookahead);
        lookahead = lexan();
    }
    else
        error("Syntax error");
}
