/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Lexical analyzer
 * Due: 10/5/2017
 */

#include "global.h"
#include "symbol.h"
#include "error.h"

char lexbuf[BSIZE];
FILE *file;

int lineno = 1;
int tokenval = NONE;

void readFile(char *fileName) {
    file = fopen(fileName, "r");
    if (!file) error(strcat(fileName, "File failed to open"));
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

int lexan() {
    int t;
    while ((t = getc(file)) != EOF) {

        // Here is how the lexer allows for comments
        if (t == '/') {
            if ((t = getc(file)) == '/') {
                while (t != '\n' && t != EOF) {
                    t = getc(file);
                }
                ungetc(t, file);
            } else {
                ungetc(t, file);
                return '/';
            }
        }
        if (t == ' ' || t == '\t');
        else if (t == '\n') {
            lineno++;
        } else if (isdigit(t)) {
            ungetc(t, file);
            fscanf(file, "%d", &tokenval);
            return INT;
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
            lexbuf[b] = EOS;
            ungetc(t, file);
            if (!strcmp("while", lexbuf)) return WHILE;
            else if (!strcmp("if", lexbuf)) return IF;
            else if (!strcmp("main", lexbuf)) return MAIN;
            else if (!strcmp("else", lexbuf)) return ELSE;
            else if (!strcmp("int", lexbuf) || !strcmp("constant", lexbuf)) return TYPE;
            else {
                insert (lexbuf, ID);
                return ID;
            }

        } else {
            switch (t) {
                case '<':
                case '>':
                    if ((t = getc(file)) == '=');
                    else ungetc(t, file);
                    return CONOP;
                case '=':
                    if ((t = getc(file)) == '=') {
                        return CONOP;
                    } else {
                        ungetc(t, file);
                        return '=';
                    }
                case '!':
                    if ((t = getc(file)) == '=') return CONOP;
                    else ungetc(t, file);
                    return '!';
                default:
                    return t;
            }
        }
    }
    return DONE;
}
