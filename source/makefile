final:	main.o parser.o lexer.o symbol.o error.o codegen.o
	gcc -o final main.o parser.o lexer.o symbol.o error.o codegen.o

main.o:	main.c global.h
	gcc -c main.c

parser.o:parser.c global.h
	gcc -c parser.c

lexer.o:lexer.c global.h
	gcc -c lexer.c

symbol.o:symbol.c global.h
	gcc -c symbol.c

error.o:error.c global.h
	gcc -c error.c

codegen.o:codegen.c global.h
	gcc -c codegen.c
clean: 
	rm final *.o
