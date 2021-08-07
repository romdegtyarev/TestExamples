#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INTEGER_CNS          1
#define DOUBLE_CNS           2
#define CHAR_CNS             3
#define STRING_MAN_LEN       256
#define FILE_NAME_MAX_LEN    256


struct dataSpace {
    void    *data_PTR;
    int      size;
    int      type;
};


typedef struct dataSpace dataSpace_T;


int main(int argc, char** argv) {
    dataSpace_T    *myData_PTR;
    int             count;
    int             number = 5;
    double          dNumber = 0.1;
    char            word[] = "Heelp";
    int            *int_PTR;
    double         *double_PTR;
    char           *char_PTR;
    FILE           *f;
    char            ch[STRING_MAN_LEN];
    char            file_name[FILE_NAME_MAX_LEN];
    int             len = 0;

    if (argc < 1) {
        printf("Use ./prog <path to file>");
    }
    len = strlen(argv[1]);
    if (len > FILE_NAME_MAX_LEN) {
        len = FILE_NAME_MAX_LEN;
    }
    strncpy(file_name, argv[1], len);
    if ((f = fopen("TESTF", "r")) == NULL) {
        printf("Невозможно открыть файл.\n");
        exit(-1);
    }

    while (!feof(f)) {
        if (fgets(ch, 256, f)) {
            printf("%s\n", ch);
        }
    }

    if (fclose(f)) {
        printf("Ошибка при закрытии файла.\n");
        exit(-1);
    }

    return 0;

    printf("Enter count of elements\n");
    scanf("%d", &count);

    myData_PTR = (dataSpace_T*)malloc(sizeof(dataSpace_T) * count);

    myData_PTR[0].data_PTR = &number;
    myData_PTR[0].size = sizeof(number);
    myData_PTR[0].type = INTEGER_CNS;

    myData_PTR[1].data_PTR = &dNumber;
    myData_PTR[1].size = sizeof(dNumber);
    myData_PTR[1].type = DOUBLE_CNS;

    myData_PTR[2].data_PTR = word;
    myData_PTR[2].size = sizeof(word);
    myData_PTR[2].type = CHAR_CNS;

    for (int i = 0; i < count; i++) {
        printf("\nType %d Poz %d\n", myData_PTR[i].type, i);
        switch (myData_PTR[i].type) {
            case INTEGER_CNS:
                int_PTR = (int *)myData_PTR[i].data_PTR;
                printf("Data %d Size %d Poz %d\n", *int_PTR, myData_PTR[i].size, i);
                break;
            case DOUBLE_CNS:
                double_PTR = (double *)myData_PTR[i].data_PTR;
                printf("Data %f Size %d Poz %d\n", *double_PTR, myData_PTR[i].size, i);
                break;
            case CHAR_CNS:
                char_PTR = (char *)myData_PTR[i].data_PTR;
                printf("Data %s Size %d Poz %d\n", char_PTR, myData_PTR[i].size, i);
                break;
        }
    }

    return 0;
}


