#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
//#include <mcheck.h>


/*
 * Exercise 6
 * Implement a buddy allocator with the following interface:
 */
typedef struct buddy_mem {
    void                  *segment;
    size_t                 size;
    bool                   is_used;
    struct buddy_mem      *next;
} buddy_mem_t;


typedef struct buddy_allocator {
    void           *segment;
    size_t          size;
    size_t          buddy_size;
    buddy_mem_t    *mem;
} buddy_allocator_t;


/**
* Create a buddy allocator
* @param raw_memory Backing memory
* @param memory_size Backing memory size
* @return the new buddy allocator
*/
buddy_allocator_t *buddy_allocator_create(void *raw_memory, size_t raw_memory_size) {
    buddy_allocator_t    *buddy_allocator;
    buddy_mem_t          *mem;


    if (raw_memory == NULL) {
        printf("buddy_allocator_create: error null raw_memory\n");
        return NULL;
    }
    buddy_allocator = malloc(sizeof(buddy_allocator_t));
    if (buddy_allocator == NULL) {
        printf("buddy_allocator_create: error null buddy_allocator\n");
        return NULL;
    }
    memset(buddy_allocator, 0, sizeof(buddy_allocator_t));
    buddy_allocator->segment = raw_memory;
    buddy_allocator->size = raw_memory_size;
    mem = malloc(sizeof(buddy_mem_t));
    if (mem == NULL) {
        printf("buddy_allocator_create: error null mem\n");
        return NULL;
    }

    buddy_allocator->buddy_size = pow(2, floor(log2(raw_memory_size)));
    buddy_allocator->mem = mem;
    mem->segment = raw_memory;
    mem->size = pow(2, floor(log2(raw_memory_size)));;
    mem->is_used = false;
    mem->next = NULL;

    printf("buddy_allocator_create: success: size: %zu: buddy_size: %zu\n\n\n", buddy_allocator->size, buddy_allocator->buddy_size);
    return buddy_allocator;
}


/**
* Destroy a buddy allocator
* @param buddy_allocator
*/
void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator) {
    buddy_mem_t    *buddy_mem_temp;


    buddy_mem_temp = buddy_allocator->mem;
    while (buddy_mem_temp != NULL) {
        /* I think this check is necessary */
        if (buddy_mem_temp->is_used == true) {
            printf("buddy_allocator_destroy: cant destroy buddy during existing allocated memory\n");
            return;
        }
        buddy_mem_temp = buddy_mem_temp->next;
    }
    free(buddy_allocator->mem);
    free(buddy_allocator);
}


/**
* Allocate memory
* @param buddy_allocator The buddy allocator
* @param size Size of memory to allocate
* @return pointer to the newly allocated memory, or @a NULL if out of memory
*/
void *buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size) {
    buddy_mem_t    *buddy_mem_temp = NULL;
    buddy_mem_t    *buddy_mem_next = NULL;
    buddy_mem_t    *buddy_mem_prev = NULL;
    buddy_mem_t    *buddy_mem_first_prev = NULL;
    void           *allocated_seg = NULL;
    size_t          buddy_size = 0;
    int             buddy_split = 0;
    int             i;
    int             temp_size;


    if (buddy_allocator == NULL) {
        printf("buddy_allocator_alloc: error: null buddy_allocator\n");
        return NULL;
    }

    buddy_size = pow(2, ceil(log2(size)));

    buddy_mem_temp = buddy_allocator->mem;
    while (buddy_mem_temp->is_used == true &&
           buddy_mem_temp->size <= buddy_size) {
        //printf("buddy_allocator_alloc: while: %p\n", buddy_mem_temp->segment);
        buddy_mem_temp = buddy_mem_temp->next;
        if (buddy_mem_temp == NULL) {
            printf("buddy_allocator_alloc: error: null buddy_mem_temp\n");
            return NULL;
        }
    }

    buddy_split = log2(buddy_mem_temp->size) - log2(buddy_size);
    allocated_seg = buddy_mem_temp->segment;
    buddy_mem_temp->size = buddy_size;
    buddy_mem_temp->is_used = true;
    buddy_mem_prev = buddy_mem_temp;
    buddy_mem_first_prev = buddy_mem_temp->next;


    for (i = 0; i < buddy_split; i++) {

        buddy_mem_next = malloc(sizeof(buddy_mem_t));
        if (buddy_mem_next == NULL) {
            printf("buddy_allocator_alloc: error null mem\n");
            return NULL;
        }
        memset(buddy_mem_next, 0, sizeof(buddy_mem_t));
        temp_size = (i +1) * buddy_size;
        buddy_mem_next->segment = allocated_seg + temp_size;
        buddy_mem_next->size = temp_size;
        buddy_mem_next->is_used = false;
        buddy_mem_prev->next = buddy_mem_next;
        buddy_mem_prev = buddy_mem_next;
    }
    if (buddy_split) {
        buddy_mem_next->next = buddy_mem_first_prev;
    }
    printf("buddy_allocator_alloc: success: segment: %p buddy: %p next: %p\n", allocated_seg, buddy_mem_temp, buddy_mem_temp->next);
    return allocated_seg;
}


void buddy_allocator_merge(buddy_allocator_t *buddy_allocator, buddy_mem_t *buddy_mem_temp, void *ptr) {
    buddy_mem_t    *buddy_mem_buddy = NULL;
    buddy_mem_t    *buddy_mem_merge_candidate = NULL;
    size_t          k;
    size_t          virtual;
    size_t          p;
    size_t          b;


    /* Find buddy */
    /* I just take this from "The Art of Computer Programming "*/
    /* If (P mod 2^(k+1) = 0) then (B = P + 2k) else (B = P − 2k) */
    /* Same P ^ (1 << k) */
    k = log2(buddy_mem_temp->size);
    virtual = (intptr_t)ptr - (intptr_t)buddy_allocator->segment;
    p = virtual ^ (1 << k);
    b = p + (intptr_t)buddy_allocator->segment;


    buddy_mem_buddy = buddy_allocator->mem;
    while (buddy_mem_buddy != NULL) {
        printf("buddy_allocator_merge: while buddy: %lx\n", b);
        if (buddy_mem_buddy->segment == (void *)b) {
            break;
        }
        buddy_mem_buddy = buddy_mem_buddy->next;
    }
    if (buddy_mem_buddy == NULL) {
        printf("buddy_allocator_merge: cant find the segment\n");
        return;
    }

    if (buddy_mem_buddy->is_used == true ||
        buddy_mem_buddy->size != buddy_mem_temp->size) {
        printf("buddy_allocator_merge: cant merge\n");
        return;
    }
    if (b > (intptr_t)ptr) {
        buddy_mem_temp->next = buddy_mem_buddy->next;
        buddy_mem_temp->size = buddy_mem_temp->size + buddy_mem_buddy->size;
        ptr = buddy_mem_temp->segment;
        buddy_mem_merge_candidate = buddy_mem_temp;
        free(buddy_mem_buddy);
    } else {
        buddy_mem_buddy->next = buddy_mem_temp->next;
        buddy_mem_buddy->size = buddy_mem_buddy->size + buddy_mem_temp->size;
        ptr = buddy_mem_buddy->segment;
        buddy_mem_merge_candidate = buddy_mem_buddy;
        free(buddy_mem_temp);
    }
    printf("before rec: buddy_mem_merge_candidate: %p %p\n", buddy_mem_merge_candidate->segment, ptr);
    buddy_allocator_merge(buddy_allocator, buddy_mem_merge_candidate, ptr);
}


/**
* Deallocates a perviously allocated memory area.
* If @a ptr is @a NULL, it simply returns
* @param buddy_allocator The buddy allocator
* @param ptr The memory area to deallocate
*/
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr) {
    buddy_mem_t    *buddy_mem_temp = NULL;


    if (buddy_allocator == NULL ||
        ptr == NULL) {
        printf("buddy_allocator_free: error: null buddy_allocator\n");
        return;
    }

    buddy_mem_temp = buddy_allocator->mem;
    while (buddy_mem_temp != NULL) {
        printf("buddy_allocator_free: while: %p %p\n", buddy_mem_temp->segment, ptr);
        if (buddy_mem_temp->segment == ptr) {
            break;
        }
        buddy_mem_temp = buddy_mem_temp->next;
    }
    if (buddy_mem_temp == NULL) {
        printf("buddy_allocator_free: cant find the segment\n");
        return;
    }

    buddy_mem_temp->is_used = false;

    buddy_allocator_merge(buddy_allocator, buddy_mem_temp, ptr);
}


void buddy_debug_mem_print(buddy_allocator_t *buddy_allocator) {
    buddy_mem_t    *buddy_mem_temp = NULL;


    if (buddy_allocator == NULL) {
        printf("buddy_debug_mem_print: error: null buddy_allocator\n");
        return;
    }

    buddy_mem_temp = buddy_allocator->mem;
    while (buddy_mem_temp != NULL) {
        printf("buddy_debug_mem_print: while: %p %lu %d %p\n",
               buddy_mem_temp->segment,
               buddy_mem_temp->size,
               buddy_mem_temp->is_used,
               buddy_mem_temp->next);
        buddy_mem_temp = buddy_mem_temp->next;
    }
}


int main(int argc, char **argv) {
    void                 *mem = NULL;
    buddy_allocator_t    *pool = NULL;
    void                 *test_1 = NULL;
    void                 *test_2 = NULL;
    void                 *test_3 = NULL;
    void                 *test_4 = NULL;


    //mtrace();
    mem = malloc(64);
    if (mem == NULL) {
        return -1;
    }
    pool = buddy_allocator_create(mem, 64);
    if (pool == NULL) {
        return -1;
    }

    /* Just my test section */
    test_1 = buddy_allocator_alloc(pool, 10);
    printf("After alloc 1\n\n");
    test_2 = buddy_allocator_alloc(pool, 10);
    printf("After alloc 2\n\n");
    test_3 = buddy_allocator_alloc(pool, 10);
    printf("After alloc 3\n\n");
    test_4 = buddy_allocator_alloc(pool, 10);
    printf("After alloc 4\n\n");

    printf("------\n\n");
    buddy_debug_mem_print(pool);
    printf("------\n\n");


    buddy_allocator_free(pool, test_2);
    printf("After free 2\n\n");
    buddy_allocator_free(pool, test_3);
    printf("After free 3\n\n");

    printf("------\n\n");
    buddy_debug_mem_print(pool);
    printf("------\n\n");


    buddy_allocator_free(pool, test_4);
    printf("After free 4\n\n");

    printf("------\n\n");
    buddy_debug_mem_print(pool);
    printf("------\n\n");


    buddy_allocator_free(pool, test_1);
    printf("After free 4\n\n");

    printf("------\n\n");
    buddy_debug_mem_print(pool);
    printf("------\n\n");
    buddy_allocator_destroy(pool);

    //muntrace();
    return 0;
}

