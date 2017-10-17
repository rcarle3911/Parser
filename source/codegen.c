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
#include "error.h"
#include "parser.h"
#include "symbol.h"

FILE *out;
int r = 0;

openFile(char s[]) {
    int len = strlen(s);
    if (len > STRMAX) error("Filename exceeds maximum allowable length");
    char strbuf[BSIZE];
    int i = 0;
    for (i = 0; i < len; i++) {
        strbuf[i] = s[i];
        if (s[i] == '.') break;
    }
    strbuf[i + 1] = 'o';
    strbuf[i + 2] = EOS;

    out = fopen(strbuf, "w");
    if (!out) error(strcat(strbuf, " File failed to write"));
}

writeExp(struct pnode *exp) {
    //create register, assign it the expression, assign exp node the register value.
    struct pnode *operator = exp->right;
    struct pnode *operand = exp->right->right;
    r++;
    fprintf(out, "r%d := %s %s %s\n", r, exp->value, operator->value, operand->value);
    exp->type = REGISTER;
    sprintf(exp->value, "r%d", r);
    exp->right = operand->right;
    free(operator);
    free(operand);
}

char* expression(struct pnode *exp) {
    struct pnode* root = exp;

    //reduce paranthesis
    while (exp->left || exp->right) {
        if (exp->left) {
            exp->type = REGISTER;
            strcpy(exp->value, expression(exp->left));
        } else {
            exp = exp->right;
        }
    }
    exp = root;

    //reduce * / %
    while (exp->right) {
        if (exp->right->type == MULTDIVOP) {
            writeExp(exp);
        } else {
            exp = exp->right;
        }
    }
    exp = root;

    //reduce + -
    while (exp->right) {
        if (exp->right->type == ADDSUBOP) {
            writeExp(exp);
        } else {
            exp = exp->right;
        }
    }
    return exp->value;
}

void declaration(struct pnode *dec) {
    struct pnode *type = dec->left->left;
    struct pnode *id = dec->left->right;
    struct pnode *exp = dec->right;

    //struct entry* sym = getSym(hash(dec->left->right->value));
    if (lookup(id->value)) {
        error("Illegal redeclaration of variable");
    } else {
        insert(id->value, id->type);
        fprintf(out, ".word %s\n", id->value);    //This should have the variable name
    }
    if (exp) {                       //This checks if an assignment is present
        fprintf(out, "%s := %s", id->value, expression(exp));
    }
}

void statements(struct pnode* root) {
    switch(root->left->type) {
        case DECLARATION:
            declaration(root->left);
            break;
        case ASSIGNMENT:
        case IF:
        case WHILE:
            break;
        default:
            error("Codegenerator failed");
    }
    if (root->right) {
        statements(root->right);
    }
}

program(struct pnode* prgrm) {
    fprintf(out, "main:\n");
    statements(prgrm->left);
    fclose(out);
}