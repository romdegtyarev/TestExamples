default : main.o
	gcc -std=c99 -Wall -o fnmatch main.o

main.o : main.c
	gcc -std=c99 -Wall -c main.c

clean :
	rm -f fnmatch main.o
