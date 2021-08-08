#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>

int main(int argc, char **argv) {
    pid_t *pid;
    int temp, status, *pidN;
    FILE *f;
    sem_t mutex;

    if (argc == 1) {
        printf("Use %s counOfProcess\n", argv[0]);
        exit(-1);
    }
    temp = atoi(argv[1]);
    if (temp == 0) {
        exit(-1);
    }
    pidN = (int*)calloc(sizeof(int), temp);
    pid = (pid_t*)calloc(sizeof(pid_t), temp);
    if ((f = fopen("output", "w")) == NULL) {
        printf("Open file error\n");
        exit(-1);
    }
    if (sem_init(&mutex, 1, 1) < 0) {
        printf("Semaphore initilization error");
        exit(-1);
    }

    for (int i = 0; i < temp; i++) {

        switch (pid[i] = fork()) {
            case -1:
                printf("Fork initilization error\n");
                exit(-1);
            case 0:
                pidN[i] = getpid();
                printf("Process %d started\n", pidN[i]);
                sem_wait(&mutex);
                fprintf(f, "%d\n", pidN[i]);
                if (fclose(f)) {
                    printf("CLose file error\n");
                    exit(-1);
                }
                sem_post(&mutex);

                sleep(10);
                if ((f = fopen("./output", "r")) == NULL) {
                    printf("Open file error\n");
                    exit(-1);
                }
                int *m;
                m = (int*)calloc(sizeof(int), temp);

                sem_wait(&mutex);
                for (int i = 0; i < temp; i++) {
                    fscanf(f, "%d", &m[i]);
                }
                if (fclose(f)) {
                    printf("CLose file error\n");
                    exit(-1);
                }
                sem_post(&mutex);

                int randPid;
                srand(getpid());
                do {
                    randPid = rand()%temp;
                } while(m[randPid] == pidN[i]);
                kill(m[randPid], SIGKILL);
                printf("Process %d kil by %d\n", m[randPid], pidN[i]);
                exit(pidN[i]);
        }
    }

    for (int i = 0; i < temp; i++) {
        if (pid[i] == (waitpid(pid[i], &status, 0))) {
            printf("Closed %d \n", WEXITSTATUS(status));
        }
    }

    sem_destroy(&mutex);
    return 0;
}
