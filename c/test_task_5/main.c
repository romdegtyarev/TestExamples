#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>


/*
 * Exercise 5
 * Implement a Linux program that uses the fnmatch() function to select lines from
 * a text file matching a pattern Example usage:
 * % fnmatch --pattern 'test*'input.txt > output.txt
 */
#define INPUT_KEY_NAME            "--pattern"
#define INPUT_KEY_NAME_LEN_MAX    256
#define STRING_LEN_MAX            2048


int fnmatch_main(char *pattern, char *file_name_STR) {
    /* Local variables */
    FILE    *file_PTR = NULL;
    char     string_STR[STRING_LEN_MAX] = {0};

    /* Function Logic */
    if (file_name_STR == NULL ||
        pattern == NULL) {
        printf("fnmatch_main: NULL file\n");
        return(-1);
    }
    if ((file_PTR = fopen(file_name_STR, "r")) == NULL) {
        printf("fnmatch_main: Cannot open file \"%s\"\n", file_name_STR);
        return(-1);
    }

    while (!feof(file_PTR)) {
        if (fgets(string_STR, STRING_LEN_MAX, file_PTR)) {
            if (!fnmatch(pattern, string_STR, 0)) {
                printf("%s", string_STR);
            }
        }
    }

    if (fclose(file_PTR)) {
        printf("fnmatch_main: Cannot close file \"%s\"\n", file_name_STR);
        return(-1);
    }
    return (0);
}


int main(int argc, char **argv) {
    /* Local variables */
    char    input_key[INPUT_KEY_NAME_LEN_MAX] = {0};

    /* Function Logic */
    if (argc < 4) {
        printf("Usage: fnmatch --pattern 'test*'input.txt\n");
        return 0;
    }
    if (!strcmp(argv[1], INPUT_KEY_NAME) &&
        argv[2] != NULL) {

        strncpy(input_key, argv[2], INPUT_KEY_NAME_LEN_MAX);
    } else {
        printf("Invalid param: %s\n", argv[1]);
        return 0;
    }
    fnmatch_main(input_key, argv[3]);

    return 0;
}

