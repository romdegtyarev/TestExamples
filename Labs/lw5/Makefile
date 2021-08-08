lw5 : lw5.o libsqrsqrt.a
	gcc -o lw5 lw5.o -L. -lsqrsqrt

lw5.o : lw5.c
	gcc -std=c99 -c lw5.c

libsqrsqrt.a : sqrt.o sqr.o
	ar rc libsqrsqrt.a sqrt.o sqr.o

sqrt.o : sqrt.c
	gcc -std=c99 -c sqrt.c -lm

sqr.o : sqr.c
	gcc -std=c99 -c sqr.c

clean :
	rm -f *.o lw5 libsqrsqrt.a