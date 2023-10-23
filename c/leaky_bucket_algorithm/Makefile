default : leaky_bucket_algorithm.o
	gcc -std=c99 -Wall -o leaky_bucket_algorithm leaky_bucket_algorithm.o

leaky_bucket_algorithm.o : leaky_bucket_algorithm.c
	gcc -std=c99 -Wall -c leaky_bucket_algorithm.c

clean :
	rm -f leaky_bucket_algorithm leaky_bucket_algorithm.o
