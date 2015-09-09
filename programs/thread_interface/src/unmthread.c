#include "unmthread.h"
#include <assert.h>



/* main context intialized statically */
static ucontext_t main_context;


/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo){
  debug("creating thread");
  if(!queue){queue = g_queue_new();}
  assert(queue);
  g_queue_push_tail(queue, thr);
  return 0;
}

/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){
  debug("yielding thread");
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
  /* wrong; need to handle differently */
  return (unmthread_t *) g_queue_peek_head(queue);
}

/* Basic condition primitives a la pthreads */
int unmthread_cond_init(unmcond_t *c)
{
  return  0;
}


int unmthread_cond_wait(unmcond_t *c, unmmutex_t *m)
{
  return  0;
}


int unmthread_cond_signal(unmcond_t *c)
{
  return  0;
}


int unmthread_cond_uninit(unmcond_t *c)
{
  return  0;
}


/* Basic mutex primitives a la pthreads */
int unmthread_mutex_init(unmmutex_t *m)
{
  return 0;
}


int unmthread_mutex_lock(unmmutex_t *m)
{
  return  0;
}


int unmthread_mutex_unlock(unmmutex_t *m)
{
  return  0;
}


int unmthread_mutex_uninit(unmmutex_t *m)
{
  return  0;
}

void context_helper(unmthread_t *thread)
{
  ucontext_t *c = &thread->context;
  c->uc_link = 0;
  thread->stack = malloc(THREAD_STACK_SIZE);
  assert(thread->stack != 0);
  c->uc_stack.ss_sp = thread->stack;
  c->uc_stack.ss_size = THREAD_STACK_SIZE;
}


/* wrapper function to malloc a thread */
unmthread_t *thread_create(int id, status status)
{
  debug("malloc thread");
  unmthread_t *thread = (unmthread_t*) malloc(sizeof(unmthread_t));
  getcontext(&thread->context);
  thread->id = id;
  thread->status = status;
  context_helper(thread);
  return thread;
}



/* prints the thread for debuggin information */
void print_thread(unmthread_t *thread)
{
  printf("Thread ID: %d\n", thread->id);
  printf("Thread status: %d\n", thread->status);
}
