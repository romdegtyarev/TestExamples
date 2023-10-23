#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Exercise 3
 * Try to describe what this code does and spot the bug(s)
 */
/*
 * The algorithm is similar to an event handler for threads. Looks like that
 * this event handler (dispatcher) count all events on each thread and checks
 * the events for success.
 */
#define MAX_THREAD   256


static int counter[MAX_THREAD];


bool dispatcher(my_thread_t *thread, int value) {
    if (process_event(thread ? thread->id : -1, value) != ERR_OK) {
        return false;
    }
    /* Return does not exist. I've added this one */
    return true;
}

int process_event(int thread_id, int result) {
    /*
     * Add check for thread_id it could be invalid. In this case memory will
     * corrupted on operation "counter[thread_id]++"
     */
    if (thread_id < 0 ||
        thread_id > MAX_THREAD) {
        return ERR_FAILED;
    }
    counter[thread_id]++;
    /* What in case of overflow? */

    switch (result) {
        case 0:
            return ERR_OK;
        case 1:
        case 2:
            return ERR_FAILED;
        /* Added for another cases */
        default:
            return ERR_FAILED;
    }
}

int main(int argc, char **argv) {
    return 0;
}

