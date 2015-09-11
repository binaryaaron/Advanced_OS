/* UNM Threads, a simple user-level thread interface that can be used to manage concurrency
 * and scheduling. All routines return 0 on success and non-zero on failure */

#ifndef _UNMTHEAD_H_
#define _UNMTHEAD_H_

struct unmthread; /* This type should be fully defined in unmthread_priv.h */
struct unmcond;
struct unmmutex;

typedef struct unmthread unmthread_t;
typedef struct unmcond unmcond_t;
typedef struct unmmutex unmmutex_t;

typedef void * (thrfunc_t)(void *arg);

/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo);

/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void);

/* Wait for termination of the supplied thread, returning its provided return value
 * to the caller in the retval argument */
int unmthread_join(unmthread_t *thr, void **retval);

/* Terminate execution of this thread, returning the provided argument to a thread that 
 * joins with this thread */
int unmthread_exit(void *retval);

/* Return a pointer to the currently running thread. Frequently used to look up thread-specific
 * information in a hash table */
unmthread_t *unmthread_current(void);

/* Basic condition primitives a la pthreads */
int unmthread_cond_init(unmcond_t *c);
int unmthread_cond_wait(unmcond_t *c, unmmutex_t *m);
int unmthread_cond_signal(unmcond_t *c);
int unmthread_cond_uninit(unmcond_t *c);

/* Basic mutex primitives a la pthreads */
int unmthread_mutex_init(unmmutex_t *m);
int unmthread_mutex_lock(unmmutex_t *m);
int unmthread_mutex_unlock(unmmutex_t *m);
int unmthread_mutex_uninit(unmmutex_t *m);

/* Include private implementation details */
#include <unmthread_priv.h>

#endif /* _UNMTHREAD_H_ */
