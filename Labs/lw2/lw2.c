#include <stdio.h>
#include <string.h>
#include <mcheck.h>

#define MAX_LEN 1024

char **readMas(int);
char **sortMas(char **, int);
void freeMas(char **, int);
void printMas(char **, int);

int main(int argc, char** argv) {
    int count;
    char **mas;
    mtrace(); //Логи выделения памяти
    printf("Enter strings count\n");
    scanf("%d", &count);
    printf("Enter %d strings\n", count);
    mas = readMas(count);
    printf("After sort\n");
    mas = sortMas(mas, count);
    printMas(mas, count);
    freeMas(mas, count);
    return 0;
}

char **readMas(int count) {
    char buffer_string[MAX_LEN]; //Строка
    char **mas; //Массив строк
    mas = (char **)malloc(sizeof(char*) * count);

    for (int i = 0; i < count ; i++) {
        scanf("%s", buffer_string);
        mas[i] = (char *)malloc(sizeof(char) * strlen(buffer_string));
        strcpy(mas[i], buffer_string);
    }

    return mas;
}

void freeMas(char **mas, int count) {

    for (int i = 0; i < count; i++) {
        free(mas[i]);
    }

    free(mas);
}

char **sortMas(char **mas, int count) {
    int flag;
    char buf[MAX_LEN];

    for (int i = 0; i < (count - 1); i++) {
        flag = strcmp(mas[i], mas[i+1]);
        if (flag > 0) {
            strcpy(buf, mas[i]);
            strcpy(mas[i], mas[i+1]);
            strcpy(mas[i+1], buf);
        }
    }

    return mas;
}

void printMas(char **mas, int count) {

    for (int i = 0; i < count ; i++) {
        printf("%s\n", mas[i]);
    }

}


