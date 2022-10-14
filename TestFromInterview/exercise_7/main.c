#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "process_event.h"


#define NUM_THREADS    10


/*
 * Exercise 7
 * Describe the bugs in the following code if any:
 */
static int *num_events;


static void * thread_run(void *arg) {
    int *thread_id = (int *)arg;

    while (num_events[*thread_id]++ < 1e6) {
        process_event();
    }
    return NULL;
}


int main(void) {
    /* We need more than one thread */
    pthread_t    t[NUM_THREADS];
    int          thread_args[NUM_THREADS];

    num_events = (int *)malloc(NUM_THREADS * sizeof(int));
    if (!num_events) {
        exit(1);
    }

    for (int j = 0; j < NUM_THREADS; j++) {
        thread_args[j] = j;
        if (pthread_create(&t[j], NULL, thread_run, &thread_args[j]) != 0) {
            exit(1);
        }
    }

    /* What about join? */
    for (int j = 0; j < NUM_THREADS; j++) {
        if (pthread_join(t[j], NULL) != 0) {
            exit(1);
        }
    }
    free(num_events);

    return 0;
}


