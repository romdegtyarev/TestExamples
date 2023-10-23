#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Exercise 2
 * Find the bug(s) in this code
 */
typedef struct {
    int param1;
    int param2;
    int param3;
    char arg[1024];
} meta_t;


void prepare_metadata(meta_t *meta, int param1, int param2, char *arg) {
    /*
     * Error in memset must be sizeof(meta_t) not pointer.
     * I think that lack of round brackets its kind of misclick.
     * Also I add size checking for arg.
     * What about param3?
     */
    int arg_size = strlen(arg);

    if (arg_size >= 1024) {
        arg_size = 1023;
    }
    memset(meta, 0, sizeof(meta_t));
    meta->param1 = param1;
    meta->param2 = param2;
    strncpy(meta->arg, arg, arg_size);
}


int main(int argc, char **argv) {
    meta_t    meta;

    prepare_metadata(&meta, 1, 2, "Test_1");
    printf("Meta: %d %d %d %s\n", meta.param1, meta.param2, meta.param3, meta.arg);
    return 0;
}

