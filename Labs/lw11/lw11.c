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
#include <semaphore.h>

sem_t mutex;

void mineCraft (int);

int main(int argc, char **argv) {
    printf("TCP SERVER\n");

    int server_sock, client_sock, port, pid;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    if (argc < 2) {
        printf("Use %s port\n", argv[0]);
        exit(-1);
    }
    port = atoi(argv[1]);
    if (sem_init(&mutex, 1, 1) < 0) {
        printf("Error Semaphore initilization");
        exit(-1);
    }
    //Cоздание сокета
    //AF_INET - Сокет Интернета
    //SOCK_STREAM - Потоковый сокет (с установкой соединения)
    //0 - По умолчанию выбирается TCP протокол
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    //Ошибка при создании сокета
    if (server_sock < 0) {
        printf("ERROR Opening socket");
    }
    //Заполенние структуры serv_addr
    //Занулим serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //Связывание сокета с локальным адресом
    serv_addr.sin_family = AF_INET;
    //Cервер принимает подключения на все IP-адреса
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //Установка порта
    serv_addr.sin_port = htons(port);
    //Связывания сокета с адресом
    if (bind(server_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR Binding");
    }
    //Режим ожидания запросов со стороны клиентов
    listen(server_sock, 5);
    clilen = sizeof(cli_addr);
    while (1) {
        //Функция accept создаёт для общения с клиентом новый сокет
        client_sock = accept(server_sock, (struct sockaddr *) &cli_addr, &clilen);
        if (client_sock < 0) {
            printf("ERROR Accept");
        }

        pid = fork();
        if (pid < 0) {
            printf("ERROR Fork");
        }
        if (pid == 0) {
            close(server_sock);
            mineCraft(client_sock);
            exit(0);
        } else {
            close(client_sock);
        }
    }

    close(server_sock);
    return 0;
}

void mineCraft (int sock) {
    int gold;
    int bytes_recv, goldOut; //Размер принятого сообщения
    char buff[256];
    FILE *f;
    if ((f = fopen("output", "r")) == NULL) {
        printf("Open file error\n");
        exit(-1);
    }
    fscanf(f, "%d\n", &gold);
    if (fclose(f)) {
        printf("Close file error\n");
        exit(-1);
    }
    //Отправляем
    snprintf(buff, strlen(buff), "%d", gold); //Преобразование результата выполнения в строку
    buff[strlen(buff)] = '\n'; //Добавление к сообщению символа конца строки
    //write(sock, &buff[0], strlen(&buff[0]));
    send(sock, &buff[0], strlen(&buff[0]), 0);

    while (gold > 0) {
        //Получаем
        //bytes_recv = read(sock, &buff[0], sizeof(buff));
        bytes_recv = recv(sock, &buff[0], sizeof(buff), 0);
        if (bytes_recv < 0) {
            printf("ERROR Reading from socket");
        }
        goldOut = atoi(buff);
        if (goldOut > 32) {
            return;
        }
        if ((f = fopen("output", "r")) == NULL) {
            printf("Open file error\n");
            exit(-1);
        }
        fscanf(f, "%d\n", &gold);
        if (fclose(f)) {
            printf("Close file error\n");
            exit(-1);
        }
        gold = gold - goldOut;
        printf("Gold Out: %d\n", goldOut);
        printf("Gold: %d\n", gold);
        sem_wait(&mutex);
        if ((f = fopen("output", "w")) == NULL) {
            printf("Open file error\n");
            exit(-1);
        }
        fprintf(f, "%d\n", gold);
        if (fclose(f)) {
            printf("Close file error\n");
            exit(-1);
        }
        sem_post(&mutex);
        //Отправляем
        snprintf(buff, strlen(buff), "%d", gold); //Преобразование результата выполнения в строку
        buff[strlen(buff)] = '\n'; //Добавление к сообщению символа конца строки
        //write(sock, &buff[0], strlen(&buff[0]));
        send(sock, &buff[0], strlen(&buff[0]), 0);
    }
    printf("Gold is end\n");
}
