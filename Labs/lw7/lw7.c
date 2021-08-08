#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO1 "/tmp/fifo.1"

int main(int argc, char **argv) {
    pid_t pid;
    int status = 0;
    int readfd, writefd;
    char buff[256];
    ssize_t len, n;

    mkfifo(FIFO1, 0666);
    pid = fork();
    if (-1 == pid) {
        printf("Fork error");
        exit(-1);
    } else if (0 == pid) {
        printf("CHILD:\n");
        len = strlen(argv[1]);
        printf("%d\n", len);
        writefd = open(FIFO1, O_WRONLY, 0);
        //sleep(5);
        write(writefd, argv[1], len);
        close(writefd);
        exit(status);
    } else {
        printf("PARENT:\n");
        readfd = open(FIFO1, O_RDONLY, 0);
        len = strlen(argv[1]);
        printf("%d\n", len);
        while ((n = read(readfd, buff, len)) > 0);
        printf("%s\n", buff);
        close(readfd);
    }
    return 0;
}
