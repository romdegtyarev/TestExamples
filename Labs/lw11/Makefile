all: lw11_client lw11

lw11 : lw11.o
	gcc -std=c99 -o lw11 lw11.o -lpthread

lw11_client : lw11_client.o
	gcc -std=c99 -o lw11_client lw11_client.o

lw11.o : lw11.c
	gcc -std=c99 -c lw11.c

lw11_client.o : lw11_client.c
	gcc -std=c99 -c lw11_client.c

clean :
	rm -f lw11_client lw11_client.o lw11 lw11.o