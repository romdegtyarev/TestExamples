#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    FILE *f, *fout1, *fout2;
    char word[256];
    strcpy(word, argv[2]);
    strcat(word, "\n");

    if ((f = fopen(argv[1], "r")) == NULL) {
        printf("Невозможно открыть файл.\n");
        exit(-1);
    }

    if ((fout1 = fopen("/home/rom/Eltex/lw4/textout1", "w")) == NULL) {

        printf("Невозможно открыть файл.\n");
        exit(-1);

    }

    if ((fout2 = fopen("/home/rom/Eltex/lw4/textout2", "w")) == NULL) {

      printf("Невозможно открыть файл.\n");
      exit(-1);

    }

    printf("Найденые строки:\n");
    char ch[256];
    int i = 1;

    while (!feof(f)) {
        if (fgets(ch, 256, f)) {
            int flag = strcmp(ch, word);
            if ((flag) == 0) {
                printf("#%d %s", i, ch );
                fprintf(fout1, "#%d %s", i, ch);
            }

            for (int i = 0; i < 256; i++) {
                if (ch[i] == 'r') {
                    ch[i] = '%';
                }
            }
            fprintf(fout2, "#%d %s", i, ch);
        }
        i++;
    }

    if (fclose(f)) {

        printf("Ошибка при закрытии файла.\n");
        exit(-1);

    }
    if (fclose(fout1)) {

        printf("Ошибка при закрытии файла.\n");
        exit(-1);

    }
    if (fclose(fout2)) {

        printf("Ошибка при закрытии файла.\n");
        exit(-1);

    }

    return 0;
}
