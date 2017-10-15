/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Parser
 * Due: 10/5/2017
 */

#include "parser.h"

int lookahead;

void parse() {
    lookahead = lexan();
    match(MAIN);
    statementblock();
    match(DONE);
    printf("Program is valid\n\n");

}

void statementblock() {
    match('{');
    while (lookahead != '}' && lookahead != DONE) {
        statement();
    }
    match('}');
}

void statement() {
    switch (lookahead) {
        case TYPE:
            decl();
            break;
        case ID:
            assg();
            break;
        case WHILE:
            iter();
            break;
        case IF:
            sel();
            break;
        default:
            error("Expected a statement");
    }
}

void decl() {
    match(TYPE);
    match(ID);
    if (lookahead != ';') {
        match('=');
        expr();
    }
    match(';');
}

void assg() {
    match(ID); match('=');
    expr();
    match(';');
}

void expr() {
    switch (lookahead) {
        case '(':
            match('(');
            expr();
            match(')');
            break;
        case '+':
        case '-':
            match(lookahead);
        case INT:
            match(INT);
            break;
        case ID:
            match(ID);
            break;
        default:
            error("Expected an operand");
    }
    oper();
}

// Having an operator is optional, but if it's there it must be followed by an expression.
void oper() {
    switch (lookahead) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '%':
            match(lookahead);
            expr();
        default:
            break;
    }
}

void condition() {
    match('(');
    expr();
    match(CONOP);
    expr();
    match(')');
}

void iter() {
    match(WHILE);
    condition();
    statementblock();

}

void sel() {
    match(IF);
    condition();
    statementblock();
    if (lookahead == ELSE) {
        match(ELSE);
        statementblock();
    }
}

void match(int token) {
    if (lookahead == token)
        lookahead = lexan();
    else
        error("Syntax error");
}
