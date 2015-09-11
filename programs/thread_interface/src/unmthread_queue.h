#ifndef _UNMTHREAD_QUEUE_
#define _UNMTHREAD_QUEUE_

typedef struct _queue {
    struct thread * head;
    struct thread * tail;
} queue;

static void queue_init( struct unm_queue * q );
static void enqueue( struct unmthread_t * t, struct unm_queue * q );
static unmthread_t * dequeue( struct unm_queue * q );

#endif /* _UNMTHREAD_QUEUE_ */
