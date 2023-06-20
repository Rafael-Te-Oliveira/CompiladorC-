all:
	clear
	flex scanner.l
	bison -d parser.y
	gcc -c *.c
	gcc -o exec *.o -lfl