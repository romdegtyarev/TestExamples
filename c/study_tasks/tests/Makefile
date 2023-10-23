default : select.o
	gcc -std=c99 -o select select.o

select.o : select.c
	gcc -std=c99 -c select.c

clean :
	rm -f select *.o