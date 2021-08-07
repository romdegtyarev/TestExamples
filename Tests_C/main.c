#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define SIZE    5


static int      a = 50;


static int * get_a() {
    return &a;
}


static void test_func(char *str, int *num, char (*str_2)[]) {
    int    i;

    printf("1 str addr: %x num addr: %x\n", str, num);
    str = "Hello word";
    *num = 5;
    printf("2 str addr: %x num addr: %x\n", str, num);
    for (i = 0; i < 4; i++) {
        printf("%s", *str_2);
    }
}


static void print_rand_num(int count) {
    int    i;
    int    rand_num;

    srand(time(NULL));
    for (i = 0; i < count; i++) {
        rand_num = rand() % SIZE + 1;
        printf("Number: %d\n", rand_num);
    }

}


int main( int argc, char **argv ) {
    int      num = 6;
    char    *str = "Roma";
    char     str_2[5][4] = {{0}};

    printf("str addr: %x num addr: %x\n", str, &num);
    printf("str: %s num: %d\n", str, num);
    test_func(str, &num, str_2);
    printf("str: %s num: %d\n", str, num);
    print_rand_num(SIZE);
    printf("a: %d\n", a);
    *(get_a()) = 60;
    printf("a: %d\n", a);

    return (0);
}


