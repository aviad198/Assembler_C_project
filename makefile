FirstRun:  FirstRun.o 
	gcc -g -Wall -ansi -pedantic FirstRun.o -o FirstRun
  
FirstRun.o:  FirstRun.c  functionsForAssembler.h  FirstRun.h main.c secondRun.h
	gcc -c FirstRun.c -o FirstRun.o

main.o: main.c main.h
	gcc -c main.c -o main.o
