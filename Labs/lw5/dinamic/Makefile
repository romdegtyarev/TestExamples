all: lw5_dynamic lw5

lw5_dynamic : lw5_dynamic.o libsqrsqrt.so
	gcc -o lw5_dynamic lw5_dynamic.o -ldl

lw5 : lw5.o libsqrsqrt.so
	gcc -o lw5 lw5.o -L. -lsqrsqrt -Wl,-rpath,.

lw5.o : lw5.c
	gcc -std=c99 -c lw5.c

lw5_dynamic.o : lw5_dynamic.c
	gcc -std=c99 -c lw5_dynamic.c

libsqrsqrt.so : sqrt.o sqr.o
	gcc -shared -o libsqrsqrt.so sqrt.o sqr.o

sqrt.o : sqrt.c
	gcc -std=c99 -c -fPIC sqrt.c -lm

sqr.o : sqr.c
	gcc -std=c99 -c -fPIC sqr.c

clean :
	rm -f *.o lw5 lw5_dynamic libsqrsqrt.so