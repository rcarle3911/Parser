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
#include "error.h"
#include "symbol.h"

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
                lexbuf[0] = '/';
                lexbuf[1] = EOS;
                return MULTDIVOP;
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
            lexbuf[b] = EOS;
            ungetc(t, file);
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
            int type = getType(lexbuf);
            if (type) return type;
            else return ID;
        } else {
            switch (t) {
                case '<':
                case '>':
                    lexbuf[0] = t;
                    if ((t = getc(file)) == '=') {
                        lexbuf[1] = '=';
                        lexbuf[2] = EOS;
                    } else {
                        lexbuf[1] = EOS;
                        ungetc(t, file);
                    }
                    return CONOP;
                case '=':
                    if ((t = getc(file)) == '=') {
                        lexbuf[0] = '=';
                        lexbuf[1] = '=';
                        lexbuf[2] = EOS;
                        return CONOP;
                    } else {
                        ungetc(t, file);
                        return '=';
                    }
                case '!':
                    if ((t = getc(file)) == '=') {
                        lexbuf[0] = '!';
                        lexbuf[1] = '=';
                        lexbuf[2] = EOS;
                        return CONOP;
                    }
                    else {
                        ungetc(t, file);
                        return '!';
                    }
                case '+':
                case '-':
                    lexbuf[0] = t;
                    lexbuf[1] = EOS;
                    return ADDSUBOP;
                case '*':
                case '%':
                    lexbuf[0] = t;
                    lexbuf[1] = EOS;
                    return MULTDIVOP;
                default:
                    return t;
            }
        }
    }
    return DONE;
}
