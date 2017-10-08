/**
 * Class: COP6025 20178 Advanced Programming Languages
 * Project 1
 * Author: Robert Lee Carle
 * Date: 10/1/2017
 * Platform/Compiler: CLion 2017.2.2 (MinGW version 5.0)
 * Professor: Dr. John Coffey
 * Description: Main method
 * Due: 10/5/2017
 */

#include "global.h"
#include "symbol.h"
#include "parser.h"

int main( int argc, char *argv[] ) {
    if (argc < 2) {
        //TODO: What should the program do with no arguments
        printf("No arguments received");
    } else {
        readFile(argv[1]);  //Passes the file name to lexer, so it can open the file.
        init();             //Builds the symbol table with reserved words.
        printf("Parsing... %s\n", argv[1]);
        parse();
        freeMemory();
        exit(0);
    }
}