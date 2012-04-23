ALL:
	bison -d derivative.y
	flex -o derivative.lex.c derivative.l
	gcc -o derivative derivative.tab.c derivative.lex.c der_func.c -lfl

clean:
	rm derivative derivative.lex.c derivative.tab.h derivative.tab.c
