#include "unmthread.h"
#include <assert.h>

 /* create the context */

int INIT = 0;
int THREAD_RUNNING = 0;
int THREAD_COUNTER = 0;
/* main context intialized statically */
static ucontext_t main_context;
ucontext_t curr_context;


void thread_runner(thrfunc_t f, void *arg)
{
  THREAD_RUNNING = 1;
  debug("running thread");
  /* THREAD_RUNNING = 1; */
  f(arg);
  /* THREAD_RUNNING = 0; */
}
/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */

/* initializes the queue and structures... */
void initialize()
{
   debug("intializing thread library");
   queue = g_queue_new();
}


void schedule_thread()
{
  unmthread_t *t = g_queue_pop_head();
  curr_context = CURRENT_THREAD->context;
  CURRENT_THREAD = unmthread_t;
  unmthread_yield();

}


int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo)
{
  if (!INIT) initialize();
  /* THREAD_RUNNING = 1; */
  CURRENT_THREAD = thr;
  debug("making context for thread");
  makecontext(&thr->context, (void (*)(void)) &thread_runner, 1, f);
  debug("yielding from creation");
  /* unmthread_yield(); */
  g_queue_push_tail(queue, thr);
  schedule_thread();

  /* CURRENT_THREAD = NULL; was causing dangling pointer? */
  /* g_queue_push_tail(queue, thr); */
  return 0;
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

/* simple helper function to initialize the context for a new thread */
void context_helper(unmthread_t *thread)
{
  assert(getcontext(&thread->context) == 0);
  thread->context.uc_link = 0;
  thread->stack = malloc(THREAD_STACK_SIZE);
  assert(thread->stack != 0);
  thread->context.uc_stack.ss_sp = thread->stack;
  thread->context.uc_stack.ss_size = THREAD_STACK_SIZE;
}


/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){
  if (THREAD_RUNNING){
    printf("Thread is running. Swapping to main context\n");
    debug("Thread is running. Swapping to main context");
    swapcontext(&CURRENT_THREAD->context, &main_context);
  }
  else {
    printf("Thread is not running. Swapping to main context\n");
    debug("Thread is not running. Swapping context with other thread");
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
  return 0;
}

/* Return a pointer to the currently running thread. Frequently used to look up thread-specific
 * information in a hash table */
unmthread_t *unmthread_current(void){
  /* wrong; need to handle differently */
  assert(CURRENT_THREAD);
  return CURRENT_THREAD;
}


/* prints the thread for debuggin information */
void print_thread(unmthread_t *thread)
{
  printf("Thread ID: %d\n", thread->id);
  printf("Thread status: %d\n", thread->status);
}
