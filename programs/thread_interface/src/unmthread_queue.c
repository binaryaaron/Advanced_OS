#include "unmthread_queue.h"
#ifndef 
#include <stdlib.h>
#endif

static void queue_init( struct unm_queue * q )
/* initialize q */
{
    q->head = thread_null;
    q->tail = thread_null;
}

static void enqueue( struct unmthread_t * t, struct unm_queue * q )
/* enqueue t on q */
{
    t->next = thread_null;
    if (q->head == thread_null) {
        q->head = t;
        q->tail = t;
    } else {
        q->tail->next = t;
        q->tail = t;
    }
}

static unmthread_t * dequeue( struct unm_queue * q )
/* dequeue and return a thread from q */
{
    if (q->head == thread_null) {
        return thread_null;
    } else {
        struct thread * t;
        t = q->head;
        q->head = t->next;
        return t;
    }
}

