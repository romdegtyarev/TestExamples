#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <pthread.h> 
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include "amessage.pb-c.h"

#define LEN          1024
#define K            5
#define L            5
#define h_addr       h_addr_list[0]
#define SLEEPTIME    60
#define RED          "\033[31m"
#define GREEN        "\033[32m"
#define WHITE        "\033[0m"

void serverFunction(int);
void *interpretMessage(void *);
void *interpretMessageUDP(void);

struct message {
    int type;
    int length;
    int sleepTime;
};

struct mq {
    long type;
    char data[10];
};

char udpMessage[] = "I'm waiting";
char udpMessageM[] = "Go Work!";
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int mqid, serverSocketInvateT, serverSocketInvate;
struct sockaddr_in serverAddress, serverAddressInvate, serverAddressInvateT, clientAddress;
struct message clientMessage;
struct mq mqueue;
struct msqid_ds qstatus;
