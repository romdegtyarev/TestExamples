#include <stdio.h>
#include <stdlib.h>

struct book {

    char bookName[100];
    int year;
    int pages;
    int cost;

};
typedef struct book books;

books *writeBooks(books *, int);
static int cmp(const void *, const void *);

int main(int argc, char** argv) {
    int count;
    printf("Enter count of books\n");
    scanf("%d", &count);
    books *lib;

    lib = (books *)malloc(sizeof(char) * count);
    writeBooks(lib, count);
    qsort((void*)lib, count, sizeof(books), cmp);

    for (int i = 0; i < count; i++) {
        printf("%s\n", lib[i].bookName);
    }

    return 0;
}

books *writeBooks(books *lib, int count) {

    for (int i = 0; i < count; i++) {
        printf("Enter Book Name\n");
        scanf("%s", &(lib[i].bookName));
        printf("Enter Book's year\n");
        scanf("%d", &(lib[i].year));
        printf("Enter Count of Book\n");
        scanf("%d", &(lib[i].pages));
        printf("Enter Book's cost\n");
        scanf("%d", &(lib[i].cost));
    }

    return lib;
}

static int cmp(const void *p1, const void *p2) {
    books * st1 = (books*)p1;
    books * st2 = (books*)p2;
    int minNameLen;
    if (strlen(st2->bookName) < strlen(st1->bookName)) {
        minNameLen = strlen(st2->bookName);
    } else {
        minNameLen = strlen(st1->bookName);
    }

    for (int i = 0; i < minNameLen; i++) {
        if (st1->bookName[i] - st2->bookName[i]) {
            return st1->bookName[i] - st2->bookName[i];
        }
    }

    return 0;
}
