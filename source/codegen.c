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
#include "codegen.h"
#include "error.h"
#include "symbol.h"

FILE *out;
int r = 0;
int l = 0;
int a = 0;

void writeExp(struct pnode *exp) {
    //create register, assign it the expression, assign exp node the register value.
    struct pnode *operator = exp->right;
    struct pnode *operand = exp->right->right;
    r++;
    fprintf(out, "\tr%d := %s %s %s\n", r, exp->value, operator->value, operand->value);
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
            exp->left = NULL;
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

void assignment(struct pnode *asg) {
    fprintf(out, "\t%s := %s\n", asg->left->value, expression(asg->right));
}

//Expression destroys Parse tree. Unable to reevaluate for condition.
void iterator(struct pnode *iter) {
    int Lab1 = ++l;
    int Lab2 = ++l;
    int r1 = ++r;
    int r2 = ++r;
    char* exp1 = expression(iter->left->left);
    char* exp2 = expression(iter->left->right);
    fprintf(out, "\tgoto L%d\nL%d:", Lab1, Lab2);
    statements(iter->right);
    fprintf(out, "L%d:\tr%d := %s\n", Lab1, r1, exp1);
    fprintf(out, "\tr%d := %s\n", r2, exp2);
    fprintf(out, "\tr%d := r%d %s r%d\n\tif r%d goto L%d\n", r1, r1, iter->left->value, r2, r1, Lab2);
}

void selector(struct pnode *sel) {
    int Lab1 = ++l;
    int Lab2 = ++l;
    int r1 = ++r;
    int r2 = ++r;
    char* exp1 = expression(sel->left->left);
    char* exp2 = expression(sel->left->right);
    fprintf(out, "\tr%d := %s\n", r1, exp1);
    fprintf(out, "\tr%d := %s\n", r2, exp2);
    fprintf(out, "\tr%d := r%d %s r%d\n", r1, r1, sel->left->value, r2);
    fprintf(out, "\tif r%d goto L%d\n", r1, Lab1);
    statements(sel->right->right);
    fprintf(out, "\tgo to L%d\n", Lab2);
    fprintf(out, "L%d: ", Lab1);
    statements(sel->right->left);
    fprintf(out, "L%d: ", Lab2);
}

void declaration(struct pnode *dec) {
    struct pnode *type = dec->left->left;
    struct pnode *id = dec->left->right;
    struct pnode *exp = dec->right;

    if (exp) fprintf(out, "\t%s := %s\n", id->value, expression(exp));
}

void statements(struct pnode *sblock) {
    switch(sblock->left->type) {
        case DECLARATION:
            declaration(sblock->left);
            break;
        case ASSIGNMENT:
            assignment(sblock->left);
            break;
        case IF:
            selector(sblock->left);
            break;
        case WHILE:
            iterator(sblock->left);
            break;
        case READ:
            readGen(sblock->left);
            break;
        case WRITE:
            writeGen(sblock->left);
            break;
        default:
            error("Codegenerator failed");
    }
    if (sblock->right) {
        statements(sblock->right);
    }
}

void readGen(struct pnode *rd) {
    int addr = ++a;
    fprintf(out, "\ta%d := &input\n", addr);
    fprintf(out, "\tcall readint\n");
    fprintf(out, "\t%s := rv\n", rd->value);
}

void writeGen(struct pnode *wr) {
    int addr1 = ++a;
    int r1 = ++r;
    int addr2 = ++a;
    fprintf(out, "\ta%d := &output\n", addr1);
    fprintf(out, "\tr%d := %s\n", r1, wr->value);
    fprintf(out, "\ta%d := r%d\n", addr2, r1);
    fprintf(out, "\tcall writeint\n");
    fprintf(out, "\ta%d := &output\n", addr1);
    fprintf(out, "\tcall writeln\n");
}

void program(struct pnode *prgrm) {
    fprintf(out, ".data\n");
    for (int i = 0; i < SYMMAX; i++) {
        struct entry* sym = getSym(i);
        while (sym) {
            if (sym->tokenType == ID) {
                fprintf(out, ".word %s\n", sym->lexeme);
            }
            sym = sym->next;
        }
    }
    fprintf(out, ".text\n");
    fprintf(out, "main:\n");
    statements(prgrm->left);
    fprintf(out, "\tgoto exit");
    fclose(out);
}

void openFile(char s[]) {
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
