#include "socketP.h"

int main(int argc, char **argv) {
    int portNumber, clientSocket, inputMessage, sleepTime, outputMessage, clientSocketInvate;
    struct sockaddr_in serverAddress, clientAddressInvate;
    struct hostent *server;
    struct message clientMessage;
    char buf[LEN];
    if (argc < 3) {
        printf("\033[31mUse %s hostname portNumber\033[0m\n", argv[0]);
        exit(-1);
    }
    srand(getpid());
    printf("\033[32mClient Manager started\033[0m\n");
    portNumber = atoi(argv[2]);
    //Извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("\033[31mNo such host ERROR\033[0m\n");
        exit(-1);
    }


    //UDP Socket creating
    clientSocketInvate = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocketInvate < 0) {
        perror("\033[31mUDP Socket creating ERROR\033[0m");
        exit(-1);
    }
    //Занулим clientAddressInvate
    bzero((char *) &clientAddressInvate, sizeof(clientAddressInvate));
    //Связывание сокета с локальным адресом
    clientAddressInvate.sin_family = AF_INET;
    clientAddressInvate.sin_addr.s_addr = htonl(INADDR_ANY);
    //Установка порта
    clientAddressInvate.sin_port = htons(portNumber);
    if (bind(clientSocketInvate, (struct sockaddr *) &clientAddressInvate, sizeof(clientAddressInvate)) < 0) {
        perror("\033[31mSocket binding ERROR\033[0m");
        exit(-1);
    }


    //TCP Socket creating
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("\033[31mSocket creating ERROR\033[0m");
        exit(-1);
    }
    //Занулим serverAddress
    bzero((char *) &serverAddress, sizeof(serverAddress));
    //Связывание сокета с локальным адресом
    serverAddress.sin_family = AF_INET;
    //Копирует h_length байтов из server->h_addr в serverAddress.sin_addr.s_addr
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    //Установка порта
    serverAddress.sin_port = htons(portNumber + 1);


    inputMessage = recvfrom(clientSocketInvate, buf, 1024, 0, NULL, 0);
    if (inputMessage <= 0) {
        perror("\033[31mMessage receive ERROR\033[0m");
        exit(-1);
    }
    buf[inputMessage] = '\0';
    printf("\033[32mMessage from server: \033[0m%s\n", buf);
    if (strcmp(buf, "Go Work!") == 0) {
        close(clientSocketInvate);
    }


    //Установка соединения с сервером
    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("\033[31mСonnecting ERROR\033[0m");
        exit (-1);
    }


    //Заполняем структуру
    clientMessage.length = 0;
    clientMessage.sleepTime = 0;
    clientMessage.type = 2;


    //Отправляем
    outputMessage = send(clientSocket, &clientMessage, sizeof(clientMessage), 0);
    if (outputMessage < 0) {
        perror("\033[31mMessage send ERROR\033[0m");
        exit(-1);
    }
    //Принимаем
    inputMessage = recv(clientSocket, &buf[0], sizeof(buf), 0);
    if (inputMessage < 0) {
        perror("\033[31mMessage receive ERROR\033[0m");
        exit(-1);
    }
    buf[inputMessage] = '\0';
    printf("\033[32mMessage from server: \033[0m%s\n", buf);
    sleepTime = atoi(buf);
    sleep(sleepTime);
    printf("\033[31mIm free\033[0m\n");


    close(clientSocket);
    return 0;
}
