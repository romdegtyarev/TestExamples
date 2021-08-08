#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;
FILE *f;
pthread_t *thread;
int temp;

void *thread_func(void *arg) {
    int i = *(int *) arg;
    sem_post(&mutex);
    sem_wait(&mutex);
    printf("Thread %d is running\n", i);
    fprintf(f, "%s\n", "Test");
    sem_post(&mutex);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();

    int randThread;
    srand(time(NULL) + i);
    randThread = rand()%temp;
    printf("%d\n", randThread);
    sleep(5);
    pthread_cancel(thread[randThread]);
    sleep(5);
    printf("Thread %d not closed\n", i);
    pthread_exit(NULL);
}

int main(int argc, char ** argv) {
    int result;
    if (argc == 1) {
        printf("Use %s counOfProcess\n", argv[0]);
        exit(-1);
    }
    temp = atoi(argv[1]);
    if (temp == 0) {
        exit(-1);
    }
    thread = (pthread_t*)calloc(sizeof(pthread_t), temp);

    if (sem_init(&mutex, 1, 1) < 0) {
        printf("Error Semaphore initilization");
        exit(-1);
    }
    if ((f = fopen("output", "w")) == NULL) {
        printf("Open file error\n");
        exit(-1);
    }
    for (int i = 0; i < temp; i++) {
        sem_wait(&mutex);
        result = pthread_create(&thread[i], NULL, thread_func, &i);
        if (result != 0) {
            perror("Creating thread");
            return -1;
        }
    }
    for (int i = 0; i < temp; i++) {
        result = pthread_join(thread[i], NULL);
        if (result != 0) {
            perror("Joining thread");
            return -1;
        }
    }

    if (fclose(f)) {
        printf("Close file error\n");
        exit(-1);
    }
    sem_destroy(&mutex);
    return 0;
}
