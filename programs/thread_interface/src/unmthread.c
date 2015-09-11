#include "unmthread.h"
#include <assert.h>

 /* create the context */

int INIT = 0;
int THREAD_RUNNING = 0;
int THREAD_COUNTER = 0;
/* main context intialized statically */
static ucontext_t main_context;
static ucontext_t curr_context;

/* initializes the queue and structures... */
void initialize()
{
   debug("intializing thread library");
   queue = g_queue_new();
   getcontext(&main_context);
   INIT = 1;
}


void thread_runner(thrfunc_t f, void *arg)
{
  debug("Running thread with Function pointer: %p\n", (void *) &f);
  CURRENT_THREAD->status = 1;
  CURRENT_THREAD->ret_val = f(CURRENT_THREAD->f_args);
  CURRENT_THREAD->status = 0;
  CURRENT_THREAD->done = 1;
  debug("Thread function return value: %p\n", (void *) &CURRENT_THREAD->ret_val);
  unmthread_yield();
}


/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo)
{
  if (!INIT) initialize();
  debug("malloc thread");
  thr = (unmthread_t*) malloc(sizeof(unmthread_t));
  thr->id = THREAD_COUNTER;
  THREAD_COUNTER++;
  thr->status = 0;
  thr->ret_val = NULL;
  thr->f_args = arg;
  thr->schedule_info = schedinfo;
  thr->done = 0;
  getcontext(&thr->context); /*fail if the getcontext fails*/

  thr->context.uc_stack.ss_sp = thr->stack = malloc(THREAD_STACK_SIZE);
  assert(thr->stack != 0); /* fail if we failed to allocate stack */

  thr->context.uc_link = 0; /* we are manually going to control the execution */
  thr->context.uc_stack.ss_size = THREAD_STACK_SIZE;


  debug("making context for thread: %i", thr->id);
  makecontext(&thr->context, (void (*)(void))thread_runner, 1, f);
  g_queue_push_tail(queue, thr);
  getcontext(&main_context);

  unmthread_yield();
  /* getcontext(&main_context); */
  return 0;
}


/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){

  if (!CURRENT_THREAD) {
    debug("current thread not defined; scheduling");
    CURRENT_THREAD = g_queue_pop_head(queue);
    /* getcontext(&main_context); */
  }
  if (CURRENT_THREAD->status == 1){ // running thread
    debug("attempting to yield running Thread: %i", CURRENT_THREAD->id);
    CURRENT_THREAD->status = 0;
    g_queue_push_tail(queue, CURRENT_THREAD);
    CURRENT_THREAD = g_queue_pop_head(queue);
    CURRENT_THREAD->status = 1;
    swapcontext(&main_context, &CURRENT_THREAD->context);
  }
  else{ // non-running thread
    if(CURRENT_THREAD->done){
      debug("Thread: %i : done", CURRENT_THREAD->id);
    if ( g_queue_is_empty(queue) ) {
      debug("Empty queue; returning") ;
      CURRENT_THREAD = NULL;
      setcontext(&main_context);
      return 0;
    }
      CURRENT_THREAD = g_queue_pop_head(queue);
      CURRENT_THREAD->status = 1;
      swapcontext(&main_context, &CURRENT_THREAD->context);
    }
    debug("attempting to yield Thread: %i", CURRENT_THREAD->id);
    swapcontext(&main_context, &CURRENT_THREAD->context);
  }
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
  unmthread_t * head =  g_queue_peek_head(queue);
  head->ret_val = retval;
  debug("exiting exit via yield");
  unmthread_yield();
  return 0;
}

/* Return a pointer to the currently running thread. Frequently used to look up thread-specific
 * information in a hash table */
unmthread_t *unmthread_current(void){
  /* wrong; need to handle differently */
  assert(CURRENT_THREAD);
  /* debug("returning current thread"); */
  return CURRENT_THREAD;
}


/* prints the thread for debuggin information */
void print_thread(unmthread_t *thread)
{
  printf("Thread ID: %d\n", thread->id);
  printf("Thread status: %d\n", thread->status);
}

