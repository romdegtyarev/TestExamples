#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
    int *array;     //Указатель на разделяемую память
    int shmid;      //IPC дескриптор для области разделяемой памяти
    int new = 1;    //Флаг необходимости инициализации элементов массива
    key_t key;      //IPC ключ

    //Генерируем IPC ключ из имени файла
    if ((key = ftok(argv[0], 0)) < 0) {
        printf("Cant generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Cant create shared memory\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
                printf("Cant find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    if (new) {
        array[0] = 1;
        array[1] = 0;
        array[2] = 1;
    } else {
        array[0] += 1;
        array[1] += 1;
        array[2] += 1;
    }
    printf("Array^\n%d\t%d\t%d", array[0], array[1], array[2]);

    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}
