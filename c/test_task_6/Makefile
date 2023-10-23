default : main.o
	gcc -std=c99 -Wall -o main main.o -lm

main.o : main.c
	gcc -std=c99 -Wall -c main.c

clean :
	rm -f main main.o
