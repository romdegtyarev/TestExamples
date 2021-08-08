#include "socketP.h"
#include "amessage.pb-c.c"

int main(int argc, char **argv) {
    int portNumber, serverSocket, clientSocket, clientAddressSize, threadid, threadUDP,
        broadcastPermission;
    pthread_t child, childUDP;
    if (argc < 2) {
        printf("\033[31mUse %s portNumber\033[0m\n", argv[0]);
        exit(-1);
    }
    printf("\033[32mServer started\033[0m\n");
    portNumber = atoi(argv[2]);

    //UDP Socket creating for client
    serverSocketInvate = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocketInvate < 0) {
        perror("\033[31mUDP Socket creating ERROR\033[0m");
        exit(-1);
    }
    //Занулим serverAddressInvate
    bzero((char *) &serverAddressInvate, sizeof(serverAddressInvate));
    //Связывание сокета с локальным адресом
    serverAddressInvate.sin_family = AF_INET;
    //Cервер принимает подключения на все IP-адреса
    serverAddressInvate.sin_addr.s_addr = inet_addr(argv[1]);
    //Установка порта
    serverAddressInvate.sin_port = htons(portNumber);
    broadcastPermission = 1;
    if (setsockopt(serverSocketInvate, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("\033[31mSetsockopt ERROR\033[0m");
        exit(-1);
    }
    //UDP Socket creating for clientManager
    serverSocketInvateT = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocketInvateT < 0) {
        perror("\033[31mUDP Socket creating ERROR\033[0m");
        exit(-1);
    }
    //Занулим serverAddressInvateT
    bzero((char *) &serverAddressInvateT, sizeof(serverAddressInvateT));
    //Связывание сокета с локальным адресом
    serverAddressInvateT.sin_family = AF_INET;
    //Cервер принимает подключения на все IP-адреса
    serverAddressInvateT.sin_addr.s_addr = inet_addr(argv[1]);
    //Установка порта
    serverAddressInvateT.sin_port = htons(portNumber+2);
    broadcastPermission = 1;
    if (setsockopt(serverSocketInvateT, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("\033[31mSetsockopt ERROR\033[0m");
        exit(-1);
    }
    //TCP Socket creating
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("\033[31mSocket creating ERROR\033[0m");
        exit(-1);
    }
    //Занулим serverAddress
    bzero((char *) &serverAddress, sizeof(serverAddress));
    //Связывание сокета с локальным адресом
    serverAddress.sin_family = AF_INET;
    //Cервер принимает подключения на все IP-адреса
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    //Установка порта
    serverAddress.sin_port = htons(portNumber+1);
    //Связывания сокета с адресом
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("\033[31mSocket binding ERROR\033[0m");
        exit(-1);
    }
    //Режим ожидания запросов со стороны клиентов
    listen(serverSocket, 6);
    printf("\033[32mServer Waiting for client on port %d\033[0m\n", portNumber+1);
    fflush(stdout);

    //Создание очереди
    mqid = msgget(IPC_PRIVATE, 0600|IPC_CREAT|IPC_EXCL);
    if (mqid < 0) {
        perror("\033[31mMessage queue creating ERROR\033[0m");
        exit(-1);
    }

    //Работа сервера
    while(1) {
        threadUDP = pthread_create(&childUDP, NULL, interpretMessageUDP, NULL);
        if (threadUDP != 0) {
            perror("\033[31mThread creating ERROR\033[0m");
            exit(-1);
        }
        clientAddressSize = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressSize);
        if (clientSocket < 0) {
            perror("\033[31mAccept ERROR\033[0m");
            exit(-1);
        }
        //Выделим каждому клиенту свой поток
        threadid = pthread_create(&child, NULL, interpretMessage, &clientSocket);
        if (threadid != 0) {
            perror("\033[31mThread creating ERROR\033[0m");
            exit(-1);
        }
        threadid = pthread_join(child, NULL);
        if (threadid != 0) {
            perror("\033[31mThread Joining ERROR\033[0m");
            exit(-1);
        }
        close(clientSocket);
    }

    return 0;
}

void serverFunction(int clientSocket) {
    int inputMessage, outputMessage, clientType, receivemq, sendmq;
    char buf[LEN];
    uint8_t buff[LEN];
    AMessage *msg;
    inputMessage = recv(clientSocket, &clientMessage, sizeof(clientMessage), 0);
    if (inputMessage <= 0) {
        perror("\033[31mMessage receive ERROR\033[0m");
        exit(-1);
    }
    //printf("Client type: %d\n", clientMessage.type);
    clientType = 1;

    switch(clientType) {
        case 1:
            inputMessage = recv(clientSocket, &buff[0], (clientMessage.length), 0);
            if (inputMessage <= 0) {
                perror("\033[31mMessage receive ERROR\033[0m");
                exit(-1);
            }
            size_t msg_len = inputMessage;
            printf("%d %s\n", msg_len, buff);
            msg = amessage__unpack(NULL, msg_len, buff);
            if (msg == NULL) {
                perror("\033[31mUnpacking incoming message ERROR\033[0m");
                exit(-1);
            }
            printf("\033[32m\nMessage from client: \033[0m %s\n", /*buf*/msg->message);
            amessage__free_unpacked(msg, NULL);
            mqueue.type = 1;
            sprintf(mqueue.data, "%d", clientMessage.sleepTime);
            printf ("\nMessage to queue: %s\n", mqueue.data);
            sendmq = msgsnd(mqid, &mqueue, sizeof(mqueue.data), 0);
            if (sendmq < 0) {
                perror("\033[31mMessage queue send ERROR\033[0m");
                exit(-1);
            }
            if (msgctl(mqid, IPC_STAT, &qstatus) < 0) {
                perror("\033[31mMsgctl failed\033[0m");
                exit(-1);
            }
            printf("Messages on queue: %d\n", qstatus.msg_qnum);
            printf("Last message sent by process: %3d at %s", qstatus.msg_lspid, ctime(&(qstatus.msg_stime)));
            printf("Current number of bytes on queue: %d\n", qstatus.__msg_cbytes);
            printf("Maximum number of bytes allowed on the queue: %d\n", qstatus.msg_qbytes);
            break;

        case 2:
            receivemq = msgrcv(mqid, &mqueue, sizeof(mqueue.data), 0, 0);
            if (receivemq < 0) {
                perror("\033[31mMessage queue receive ERROR\033[0m");
                exit(-1);
            }
            printf("Received message: %s\n", mqueue.data);
            sprintf(buf, "%s", mqueue.data);
            outputMessage = send(clientSocket, buf, strlen(buf), 0);
            if (outputMessage < 0) {
                perror("\033[31mMessage send ERROR\033[0m");
                exit(-1);
            }
            printf("%d\n", outputMessage);
            break;
    }
}

void *interpretMessage(void *argClientSocket) {
    int clientSocket = *(int *)argClientSocket;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
    serverFunction(clientSocket);
    pthread_exit(NULL);
}

void *interpretMessageUDP(void) {
    int message, i = 1;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
    while (1) {
        if (msgctl(mqid, IPC_STAT, &qstatus) < 0) {
            perror("\033[31mMsgctl failed\033[0m");
            exit(-1);
        }
        printf("Messages on queue: %d\n", qstatus.msg_qnum);
        if (qstatus.msg_qnum <= 1630) {
            message = sendto(serverSocketInvateT, udpMessage, sizeof(udpMessage), 0,
                (struct sockaddr *)&serverAddressInvate, sizeof(serverAddressInvate));
            if (message <= 0) {
                perror("\033[31mMessage receive ERROR\033[0m");
                exit(-1);
            }
            printf("#%d Invite client\n", i);
            sleep(K);
        }
        if (qstatus.msg_qnum > 0) {
            message = sendto(serverSocketInvateT, udpMessageM, sizeof(udpMessageM), 0,
                (struct sockaddr *)&serverAddressInvateT, sizeof(serverAddressInvateT));
            if (message <= 0) {
                perror("\033[31mMessage receive ERROR\033[0m");
                exit(-1);
            }
            printf("#%d Invite manager\n", i);
            sleep(L);
        }
        i++;
    }
    pthread_exit(NULL);
}
