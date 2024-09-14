all:
	clear
	flex scanner.l
	bison -d parser.y
	gcc -c *.c -w
	gcc -o g-- *.o -lfl 
	clear
	@echo "Compilação concluída! Execute o compilador com o comando ./g-- <nome do arquivo>.cminus"