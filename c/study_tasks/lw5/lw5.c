#include <stdio.h>

extern int mysqr();
extern double mysqrt();

int main(int argc, char** argv) {
    int a;
    printf("Введите число\n");
    scanf("%d", &a);
    printf("Квадрат числа: %d\nКорень из числа %f\n", mysqr(a), mysqrt(a));
    return 0;
}
