all: first second
	
first: main.c 
	gcc -g main.c -o main -lreadline

second: sigint.c
	gcc sigint.c -o sigint