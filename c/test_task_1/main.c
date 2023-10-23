#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Exercise 1
 * Is the code below correct? If not why?
 */
/*
 * The strcat function does not allocate memory, It only copies from one line to
 * another. Since the length of the first string is less than the length of the
 * second, the memory will overlaid.
 *
 * My version of this code.
 */
char* make_file_name(char *base) {
    char    *file_name = NULL;
    char    *file_name_suffix = ".pcap\0";
    int      base_len = 0;
    int      file_name_suffix_len = 0;

    if (base == NULL) {
        return NULL;
    }

    base_len = strlen(base);
    file_name_suffix_len = strlen(file_name_suffix);

    /* For null-terminated string +1 */
    file_name = malloc(base_len + file_name_suffix_len + 1);
    if (file_name == NULL) {
        return NULL;
    }

    memcpy(file_name, base, base_len);
    memcpy(file_name + base_len, file_name_suffix, file_name_suffix_len + 1);

    return file_name;
}


int main(int argc, char **argv) {
    char     file_name[20] = {0};
    char    *new_file_name = NULL;

    strncpy(file_name, "Test", 20);
    new_file_name = make_file_name(file_name);
    printf("Old: %s New: %s\n", file_name, new_file_name);
    free(new_file_name);

    return 0;
}

