#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define BUCKET_SIZE 10
#define STORAGE_AT_START 0

#define OUTGOING_RATE 1
#define OUTGOING_RATE_MAX 10


void lb1() {
    int    storage;
    int    incoming;
    int    outgoing;
    int    bucket_size;
    int    n;
    int    size_left;

    /* Init */
    /* Initial packets in the bucket */
    storage = STORAGE_AT_START;
    /* Total Num of packets that can be accommodated in the bucket */
    bucket_size = BUCKET_SIZE;
    /* Num of packets that exits the bucket at a time */
    outgoing = OUTGOING_RATE;

    /* Total Num of times bucket content is checked */
    printf("Enter num of inputs: ");
    scanf("%d", &n);
    /* Num of packets that enters the bucket at a time */
    printf("Enter num of packets that enters the bucket at a time: ");
    scanf("%d", &incoming);

    for (int i = 0; i < n; i++) {
        size_left = bucket_size - storage;
        if (incoming <= size_left) {
            /* Update storage */
            storage += incoming;
        } else {
            printf("Packet loss = %d\n", incoming);
        }
        printf("Buffer size= %d out of bucket size= %d\n", storage, bucket_size);
        storage -= outgoing;
    }
}


void lb2() {
    int    incoming;
    int    outgoing;
    int    bucket_size;
    int    n;
    int    storage;

    storage = STORAGE_AT_START;
    bucket_size = BUCKET_SIZE;
    outgoing = OUTGOING_RATE;

    printf("Enter num of inputs: ");
    scanf("%d", &n);

    while (n != 0) {
        incoming = rand() % OUTGOING_RATE_MAX;
        printf("Incoming packet size %d\n", incoming);
        if (incoming <= (bucket_size - storage)) {
            storage += incoming;
            printf("Bucket buffer size %d out of %d\n", storage, bucket_size);
        } else {
            printf("Dropped %d no of packets\n", incoming - (bucket_size - storage));
            printf("Bucket buffer size %d out of %d\n", storage, bucket_size);
            storage = bucket_size;
        }
        storage = storage - outgoing;
        printf("After outgoing %d packets left out of %d in buffer\n", storage, bucket_size);
        n--;
    }
}


int main() {
    srand(time(NULL));
    lb1();
    lb2();
    return 0;
}


