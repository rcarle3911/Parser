/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Implements the symbol table
 * Due: 10/26/2017
 */

#include "symbol.h"
#include "error.h"

struct entry *symtable[SYMMAX] = {0};

struct entry keywords[] = {
    NULL, "main", MAIN,
    NULL, "while", WHILE,
    NULL, "int", TYPE,
    NULL, "constant", TYPE,
    NULL, "if", IF,
    NULL, "else", ELSE,
    NULL, "", 0
};

int hash (char s[]) {
    int val = 0, len = strlen(s);
    if (len >= STRMAX) error ("Max string length exceeded");
    int i = 0;
    while (i < len) {
        if (i % 3 == 0) val *= (s[i] % 10);
        else if (i % 10 == 0) val *= (s[i] % 30);
        else val += s[i];
        val %= SYMMAX;
        i++;
    }
    return val;
}

int lookup (char s[]) {
    struct entry *current = symtable[hash(s)];
    if (!current) {
        return 0;
    } else {
        while (current) {
            if (!strcmp(s, current->lexeme)) {
                return 1;
            } else {
                current = current->next;
            }
        }
        return 0;
    }
}

int insert (char s[], int tok) {
    if (strlen(s) >= STRMAX) error("Max string length exceeded.");
    if (lookup(s));
    else {
        int index = hash(s);
        struct entry *input = (struct entry*)malloc(sizeof(struct entry));
        strcpy(input->lexeme, s);
        input->tokenType = tok;
        input->next = symtable[index];
        symtable[index] = input;
    }
}

struct entry* getSym(int i) {
    return symtable[i];
}

int init() {
    struct entry *p;
    for (p = keywords; p->tokenType; p++) {
        insert(p->lexeme, p->tokenType);
    }
}