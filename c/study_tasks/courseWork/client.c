#include "socketP.h"
#include "amessage.pb-c.c"

int main(int argc, char **argv) {
    int portNumber, clientSocket, clientSocketInvate, outputMessage, inputMessage;
    struct sockaddr_in serverAddress, clientAddressInvate;
    struct hostent *server;
    struct message clientMessage;
    char buf[LEN];
    if (argc < 3) {
        printf(RED"Use %s hostname portNumber\n"WHITE, argv[0]);
        exit(-1);
    }
    srand(getpid());
    printf(GREEN"Client started\n"WHITE);
    portNumber = atoi(argv[2]);
    //Извлечение хоста
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf(RED"No such host ERROR\n"WHITE);
        exit(-1);
    }
    //UDP Socket creating
    clientSocketInvate = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocketInvate < 0) {
        perror(RED"UDP Socket creating ERROR"WHITE);
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
        perror(RED"Socket binding ERROR"WHITE);
        exit(-1);
    }
    //TCP Socket creating
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror(RED"Socket creating ERROR"WHITE);
        exit(-1);
    }
    //Занулим serverAddress
    bzero((char *) &serverAddress, sizeof(serverAddress));
    //Связывание сокета с локальным адресом
    serverAddress.sin_family = AF_INET;
    //Копирует h_length байтов из server->h_addr в serverAddress.sin_addr.s_addr
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    //Установка порта
    serverAddress.sin_port = htons(portNumber+1);


    //Получение сообщения по UDP
    inputMessage = recvfrom(clientSocketInvate, buf, 1024, 0, NULL, 0);
    if (inputMessage <= 0) {
        perror(RED"Message receive ERROR\n"WHITE);
        exit(-1);
    }
    buf[inputMessage] = '\0';
    printf(GREEN"Message from server:"WHITE);
    printf("%s\n", buf);
    if (strcmp(buf, "I'm waiting") == 0) {
        close(clientSocketInvate);

        AMessage msg = AMESSAGE__INIT;
        //Установка TCP соединения с сервером
        if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
            perror(RED"Сonnecting ERROR"WHITE);
            exit(-1);
        }
        //Заполняем случайную строку
        msg.clienttype = 1;
        msg.length = rand()%(LEN / 2);
        msg.sleeptime = rand()%(SLEEPTIME + 1);
        for (int i = 0; i < (msg.length); i++) {
            buf[i] = letters[rand()%52];
        }
        //Protobuff
        int len;
        char *mes;
        msg.message = (char *)malloc(msg.length);
        strcpy(msg.message, buf);
        len = amessage__get_packed_size(&msg);
        mes = (char *)malloc(len);
        msg.length = len;
        amessage__pack(&msg, mes);
        //Отправляем
        outputMessage = send(clientSocket, &mes, sizeof(mes), 0);
        if (outputMessage < 0) {
            perror(RED"Message send ERROR"WHITE);
            exit(-1);
        }
        printf(GREEN"Message to server: " WHITE "%s\n\nSize: %d Sleep time: %d Client Type: %d\n",
            msg.message,  msg.length,  msg.sleeptime, msg.clienttype);
        sleep(msg.sleeptime);
        printf(RED"Exit\n"WHITE);

        free(mes);
        free(msg.message);
        close(clientSocket);
    }
    return 0;
}
