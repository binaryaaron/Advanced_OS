#include "unmthread.h"
#include <assert.h>

 /* create the context */

int INIT = 0;
int THREAD_RUNNING = 0;
int THREAD_COUNTER = 0;
/* main context intialized statically */
static ucontext_t main_context;
ucontext_t curr_context;

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
  /* THREAD_RUNNING = 1; */
  debug("Running thread with Function pointer: %p\n", (void *) &f);
  void *ret = f(arg);
  debug("Thread function return value: %p\n", (void *) &ret);
  /* THREAD_RUNNING = 0; */
  unmthread_exit(ret);
}

/* wrapper function to malloc a thread */
unmthread_t *thread_create()
{
  status _status = RUNNING;
  debug("malloc thread");
  unmthread_t *thread = (unmthread_t*) malloc(sizeof(unmthread_t));
  thread->id = THREAD_COUNTER;
  THREAD_COUNTER++;
  thread->status = _status;
  context_helper(thread);
  return thread;
}


/* convenience */
void schedule_thread()
{
  debug("scheudling thread");
  unmthread_t *t = g_queue_pop_head(queue);
  /* curr_context = CURRENT_THREAD->context; */
  CURRENT_THREAD = t;
  THREAD_RUNNING = 1;
  /* unmthread_yield(); */

}

/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo)
{
  if (!INIT) initialize();
  /* THREAD_RUNNING = 1; */
  /* CURRENT_THREAD = thr; */
  debug("making context for thread");
  debug("Function pointer: %p\n", (void *) &f);
  g_queue_push_tail(queue, thr);
  debug("making context");
  makecontext(&thr->context, (void (*)(void)) &thread_runner, 1, f);
  debug("setting context");
  setcontext(&thr->context);

  /* unmthread_yield(); */
  return 0;
}


/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){
  debug("attempting to yield");
  assert(queue);
  /* if (THREAD_RUNNING){ */
  /*   printf("Thread is running. Swapping to main context\n"); */
    schedule_thread();
    printf("Swapping thread context\n");
    swapcontext(&CURRENT_THREAD->context, &main_context);
    /* swapcontext(&main_context, &CURRENT_THREAD->context); */
    printf("setting thread status\n");
    CURRENT_THREAD->status = 1;
    /* g_queue_push_tail(queue, CURRENT_THREAD); */
  /* } */
  /* else { */
    /* debug("Thread is not running. Swapping context to thread"); */
    /* schedule_thread(); */
    /* swapcontext(&curr_context, &CURRENT_THREAD->context); */
    /* g_queue_push_tail(queue, CURRENT_THREAD); */
  /* } */
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
  assert(CURRENT_THREAD);
  debug("returning current thread");
  return CURRENT_THREAD;
}


/* prints the thread for debuggin information */
void print_thread(unmthread_t *thread)
{
  printf("Thread ID: %d\n", thread->id);
  printf("Thread status: %d\n", thread->status);
}



/* simple helper function to initialize the context for a new thread */
void context_helper(unmthread_t *thread)
{
  
  getcontext(&thread->context); /*fail if the getcontext fails*/

  thread->context.uc_stack.ss_sp = thread->stack = malloc(THREAD_STACK_SIZE);
  assert(thread->stack != 0); /* fail if we failed to allocate stack */

  thread->context.uc_link = 0; /* we are manually going to control the execution */
  thread->context.uc_stack.ss_size = THREAD_STACK_SIZE;
}

