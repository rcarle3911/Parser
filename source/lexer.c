/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Lexical analyzer
 * Due: 10/26/2017
 */

#include "global.h"
#include "lexer.h"
#include "symbol.h"
#include "error.h"

char lexbuf[BSIZE];
FILE *file;

int lineno = 1;

void readFile(char *fileName) {
    file = fopen(fileName, "r");
    if (!file) error(strcat(fileName, " File failed to open"));
}

void freeMemory() {
    //Frees any memory allocated in symbol table
    struct entry* current;
    struct entry* next;
    int i = 0;
    while (i < SYMMAX) {
        current = getSym(i);
        while (current) {
            next = current->next;
            free(current);
            current = next;
        }
        i++;
    }
    if (file) fclose(file);
}

struct token* lexan() {
    int t;
    struct token* tok;
    while ((t = getc(file)) != EOF) {

        // Here is how the lexer allows for comments
        if (t == '/') {
            if ((t = getc(file)) == '/') {
                while (t != '\n' && t != EOF) {
                    t = getc(file);
                }
                ungetc(t, file);
            } else {
                tok = malloc(sizeof(*tok));
                ungetc(t, file);
                tok->type = MULTDIVOP;
                strcpy(tok->value,"/");
                return tok;
            }
        }
        if (t == ' ' || t == '\t');
        else if (t == '\n') {
            lineno++;
        } else if (isdigit(t)) {
            int b = 0;
            while (isdigit(t)) {
                lexbuf[b] = t;
                t = getc(file);
                b++;
                if (b >= BSIZE) error("That number is way too big!");
            }
            tok = malloc(sizeof(*tok));
            lexbuf[b] = EOS;
            ungetc(t, file);
            tok->type = INT;
            strcpy(tok->value, lexbuf);
            return tok;
        } else if (isalpha(t)) {
            int b = 0;
            while (isalnum(t) || t == '_') {
                lexbuf[b] = t;
                t = getc(file);
                if (t == '_' && lexbuf[b] == '_') error("You cannot have consecutive underscores");
                b++;
                if(b >= BSIZE) error("Exceeded character buffer size");
            }
            if (lexbuf[b - 1] == '_') error("An identifier cannot end with an underscore");
            tok = malloc(sizeof(*tok));
            lexbuf[b] = EOS;
            ungetc(t, file);
            if (!strcmp("while", lexbuf)) tok->type = WHILE;
            else if (!strcmp("if", lexbuf)) tok->type = IF;
            else if (!strcmp("main", lexbuf)) tok->type = MAIN;
            else if (!strcmp("else", lexbuf)) tok->type = ELSE;
            else if (!strcmp("int", lexbuf) || !strcmp("constant", lexbuf)) tok->type = TYPE;
            else tok->type = ID;
            strcpy(tok->value, lexbuf);
            return tok;

        } else {
            tok = malloc(sizeof(*tok));
            switch (t) {
                case '<':
                    if ((t = getc(file)) == '=')
                        strcpy(tok->value, "<=");
                    else {
                        ungetc(t, file);
                        strcpy(tok->value, "<");
                    }
                    tok->type = CONOP;
                    return tok;
                case '>':
                    if ((t = getc(file)) == '=')
                        strcpy(tok->value, ">=");
                    else {
                        ungetc(t, file);
                        strcpy(tok->value, ">");
                    }
                    tok->type = CONOP;
                    return tok;
                case '=':
                    if ((t = getc(file)) == '=') {
                        tok->type = CONOP;
                        strcpy(tok->value, "==");
                    } else {
                        ungetc(t, file);
                        tok->type = '=';
                        strcpy(tok->value, "=");
                    }
                    return tok;
                case '!':
                    if ((t = getc(file)) == '=') {
                        tok->type = CONOP;
                        strcpy(tok->value, "!=");
                    }
                    else {
                        ungetc(t, file);
                        tok->type = '!';
                    }
                    return tok;
                case '+':
                    tok->type = ADDSUBOP;
                    strcpy(tok->value, "+");
                    return tok;
                case '-':
                    tok->type = ADDSUBOP;
                    strcpy(tok->value, "-");
                    return tok;
                case '*':
                    tok->type = MULTDIVOP;
                    strcpy(tok->value, "*");
                    return tok;
                case '%':
                    tok->type = MULTDIVOP;
                    strcpy(tok->value, "%");
                    return tok;
                default:
                    tok->type = t;
                    return tok;
            }
        }
    }
    tok->type = DONE;
    return tok;
}
