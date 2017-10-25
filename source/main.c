/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 2
 * Author: Robert Lee Carle
 * Date: 10/15/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Main method
 * Due: 10/26/2017
 */

#include "global.h"
#include "lexer.h"
#include "codegen.h"
#include "symbol.h"

int main( int argc, char *argv[] ) {
    if (argc < 2) {
        //TODO: What should the program do with no arguments
        printf("No arguments received");
    } else {
        readFile(argv[1]);  //Passes the file name to lexer, so it can open the file.
        openFile(argv[1]);
        init();             //Builds the symbol table with reserved words.
        printf("Parsing... %s\n", argv[1]);
        program(parse());
        freeMemory();
        freeParseTree();
        exit(0);
    }
}