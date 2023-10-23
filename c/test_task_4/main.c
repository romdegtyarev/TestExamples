#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Exercise 4
 * Implement a program called bsearch that takes as input a key and a CSV file
 * sorted by the first column, and outputs the lines with a first column matching
 * the key.
 * Example usage:
 * % bsearch --key test input.csv > output.csv
 * You must use good programming style, easy to understand code, and strive for
 * high performance.
 */
#define INPUT_KEY_NAME            "--key"
#define INPUT_KEY_NAME_LEN_MAX    256
#define STRING_LEN_MAX            2048
#define CSV_SEPARATOR             "\","


int bsearch_main(char *key, char *file_name_STR) {
    /* Local variables */
    FILE    *file_PTR = NULL;
    char     string_STR[STRING_LEN_MAX] = {0};
    char     compare_STR[STRING_LEN_MAX] = {0};

    /* Function Logic */
    if (file_name_STR == NULL ||
        key == NULL) {

        printf("bsearch_main: NULL file or key\n");
        return(-1);
    }
    if ((file_PTR = fopen(file_name_STR, "r")) == NULL) {
        printf("bsearch_main: Cannot open file \"%s\"\n", file_name_STR);
        return(-1);
    }

    while (!feof(file_PTR)) {
        if (fgets(string_STR, STRING_LEN_MAX, file_PTR)) {
            strncpy(compare_STR, string_STR, STRING_LEN_MAX);
            strtok(compare_STR, CSV_SEPARATOR);
            if (!strcmp(compare_STR + 1, key)) {
                /* Or strstr, what does key really mean? */
                printf("%s", string_STR);
            }
        }
    }

    if (fclose(file_PTR)) {
        printf("bsearch_main: Cannot close file \"%s\"\n", file_name_STR);
        return(-1);
    }
    return (0);
}


int main(int argc, char **argv) {
    /* Local variables */
    char    input_key[INPUT_KEY_NAME_LEN_MAX] = {0};

    /* Function Logic */
    if (argc < 4) {
        printf("Usage: bsearch --key test input.csv > output.csv\n");
        return 0;
    }
    if (!strcmp(argv[1], INPUT_KEY_NAME) &&
        argv[2] != NULL) {

        strncpy(input_key, argv[2], INPUT_KEY_NAME_LEN_MAX);
    } else {
        printf("Invalid param: %s\n", argv[1]);
        return 0;
    }
    bsearch_main(input_key, argv[3]);

    return 0;
}

