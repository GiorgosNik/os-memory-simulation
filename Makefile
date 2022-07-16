all: list.h list.c main.c
	gcc -c list.c -o list.o
	gcc main.c list.o -lm -o sim
 

clean:
	rm list.o
