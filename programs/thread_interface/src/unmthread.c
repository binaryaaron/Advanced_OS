#include <stdio.h>
#include "unmthread.h"
#include "dbg.h"

/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo){
  debug("creating thread");
  return 0;
}

/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){
  return 0;
}

/* Wait for termination of the supplied thread, returning its provided return value
 * to the caller in the retval argument */
int unmthread_join(unmthread_t *thr, void **retval){
  return 0;
}

/* Terminate execution of this thread, returning the provided argument to a thread that 
 * joins with this thread */
int unmthread_exit(void *retval){
  return 0;
}

/* Return a pointer to the currently running thread. Frequently used to look up thread-specific
 * information in a hash table */
unmthread_t *unmthread_current(void){
  return  *unmthread_current;
}

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



