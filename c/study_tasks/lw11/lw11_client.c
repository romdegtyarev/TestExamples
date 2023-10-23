#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define h_addr h_addr_list[0]

int goldPorc = 32, work=0;

int main(int argc, char **argv) {
    int client_sock, port, bytes_recv, gold;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[256];
    printf("TCP CLIENT\n");

    srand(getpid());
    if (argc < 3) {
        printf("Use %s hostname port\n", argv[0]);
        exit(-1);
    }
    port = atoi(argv[2]);
    //Cоздание сокета
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        printf("ERROR opening socket");
    }
    //Извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("ERROR No such host\n");
        exit(-1);
    }
    //Заполенние структуры serv_addr
    //Занулим serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //Связывание сокета с локальным адресом
    serv_addr.sin_family = AF_INET;
    //Копирует h_length байтов из server->h_addr в serv_addr.sin_addr.s_addr
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    //Установка порта
    serv_addr.sin_port = htons(port);
    //Установка соединения с сервером
    if (connect(client_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR Сonnecting\n");
        exit (-1);
    }

    bytes_recv = recv(client_sock, &buff[0], sizeof(buff) - 1, 0);
    if (bytes_recv < 0) {
        printf("ERROR Reading\n");
    }
    gold = atoi(buff);
    printf("Gold: %d\n", gold);
    while (gold > 0) {
        //Отправляем
        if (gold < goldPorc) {
            snprintf(buff, strlen(buff), "%d", gold);
            buff[strlen(buff)] = '\n';
            bytes_recv = send(client_sock, &buff[0], strlen(&buff[0]), 0);
            if (bytes_recv < 0) {
                printf("ERROR Writing\n");
            }
            work = work + gold;
        } else {
            snprintf(buff, strlen(buff), "%d", goldPorc);
            buff[strlen(buff)] = '\n';
            bytes_recv = send(client_sock, &buff[0], strlen(&buff[0]), 0);
            if (bytes_recv < 0) {
                printf("ERROR Writing\n");
            }
            work = work + goldPorc;
        }
        //Принимаем
        bytes_recv = recv(client_sock, &buff[0], sizeof(buff) - 1, 0);
        if (bytes_recv < 0) {
            printf("ERROR Reading\n");
        }
        gold = atoi(buff);
        printf("Gold: %d\n", gold);
        //Спим
        int a = rand()%8;
        sleep(a);
    }
    printf("Exit Gold Out %d", work);
    close(client_sock);
    return 0;
}
