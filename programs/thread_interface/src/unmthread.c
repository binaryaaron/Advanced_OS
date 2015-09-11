#include "unmthread.h"
#include <assert.h>


int INIT = 0;
int THREAD_RUNNING = 0;
int THREAD_COUNTER = 0;
int AM_WAITING = 0;
/* main context intialized statically */
static ucontext_t main_context;
static ucontext_t curr_context;

struct unmthread_t *tmp_thread;

/* initializes the queue and structures... */
void initialize()
{
   debug("intializing thread library");
   queue = g_queue_new();
   getcontext(&main_context);
  /* MAIN_THREAD = (unmthread_t*) malloc(sizeof(unmthread_t)); */
  /* MAIN_THREAD->id = THREAD_COUNTER; */
  /* THREAD_COUNTER++; */
  /* MAIN_THREAD->status = READY; */
  /* MAIN_THREAD->ret_val = NULL; */
  /* MAIN_THREAD->f_args = NULL; */
  /* MAIN_THREAD->schedule_info = NULL; */

  /* MAIN_THREAD->context.uc_stack.ss_sp = MAIN_THREAD->stack = malloc(THREAD_STACK_SIZE); */
  /* assert(MAIN_THREAD->stack != 0); /1* fail if we failed to allocate stack *1/ */

  /* MAIN_THREAD->context.uc_link = 0; /1* we are manually going to control the execution *1/ */
  /* MAIN_THREAD->context.uc_stack.ss_size = THREAD_STACK_SIZE; */
  /* g_queue_push_tail(queue, MAIN_THREAD); */
  /* getcontext(&MAIN_THREAD->context); */
  /* makecontext(&MAIN_THREAD->context, (void (*)(void))wait_for_threads, 0); */
  INIT = 1;
}


void thread_runner(thrfunc_t f, void *arg)
{
  debug("Running thread %i with Function pointer: %p\n",
        CURRENT_THREAD->id, (void *) &f);
  CURRENT_THREAD->status = RUNNING;
  CURRENT_THREAD->ret_val = f(CURRENT_THREAD->f_args);
  CURRENT_THREAD->status = DONE;
  /* CURRENT_THREAD->done = 1; */
  debug("Thread function return value: %p\n", (void *) &CURRENT_THREAD->ret_val);
  unmthread_yield();
}


/* Create a thread starting at the running function with the supplied argument and scheduling 
 * information. In non-preemptive code, the created thread will not actually run until the
 * current thread yields.  */
int unmthread_create(unmthread_t *thr, thrfunc_t f, void *arg, void *schedinfo)
{
  if (!INIT) initialize();
  /* debug("malloc thread"); */
  thr = (unmthread_t*) malloc(sizeof(unmthread_t));
  thr->id = THREAD_COUNTER;
  THREAD_COUNTER++;
  thr->status = READY;
  thr->ret_val = NULL;
  thr->f_args = arg;
  thr->schedule_info = schedinfo;
  getcontext(&thr->context);

  thr->context.uc_stack.ss_sp = thr->stack = malloc(THREAD_STACK_SIZE);
  assert(thr->stack != 0); /* fail if we failed to allocate stack */

  thr->context.uc_link = 0; /* we are manually going to control the execution */
  thr->context.uc_stack.ss_size = THREAD_STACK_SIZE;


  makecontext(&thr->context, (void (*)(void))thread_runner, 1, f);
  /* g_queue_push_tail(queue, thr); */
  if (!CURRENT_THREAD){
    debug("makiing first thread Thread: %i", thr->id);
    CURRENT_THREAD = thr;
  } else{
    debug("pushing Thread to queue: %i", thr->id);
    g_queue_push_tail(queue, thr);
  }
  getcontext(&main_context);

  /* getcontext(&main_context); */
  unmthread_yield();
  return 0;
}


/* Cause the current thread to yield execution to another thread if one is availble to 
 * run on this processor */
int unmthread_yield(void){

  if (CURRENT_THREAD->status == READY){ 
    debug("Starting ready thread: %i", CURRENT_THREAD->id);
    CURRENT_THREAD->status = RUNNING;
    /* swapcontext(&main_context, &CURRENT_THREAD->context); */
    setcontext(&CURRENT_THREAD->context);
    debug("HURRAY");
  } 
  /* there is a thread running */
  else if (CURRENT_THREAD->status == RUNNING){ // running thread
    int qlen;
    qlen = g_queue_get_length(queue);
    if (qlen == 1 || qlen == 0){
      CURRENT_THREAD->status = READY;
      swapcontext(&CURRENT_THREAD->context, &main_context);
    }
    else{
      debug("attempting to yield running Thread: %i", CURRENT_THREAD->id);
      CURRENT_THREAD->status = READY;
      g_queue_push_tail(queue, CURRENT_THREAD);
      TMP_THREAD = CURRENT_THREAD;
      CURRENT_THREAD = g_queue_pop_head(queue);
      /* swapcontext(&CURRENT_THREAD->context, &main_context); */
      /* swapcontext(&main_context, &TMP_THREAD->context); */
      swapcontext(&TMP_THREAD->context, &CURRENT_THREAD->context );
      CURRENT_THREAD->status = READY;
    }
  }

  else if (CURRENT_THREAD->status == BLOCKED){ // running thread
    debug("attempting to yield blocked Thread: %i", CURRENT_THREAD->id);
    g_queue_push_tail(queue, CURRENT_THREAD);
    TMP_THREAD = CURRENT_THREAD;
    CURRENT_THREAD = g_queue_pop_head(queue);
    CURRENT_THREAD->status = RUNNING;
    /* swapcontext(&TMP_THREAD->context, &CURRENT_THREAD->context ); */
    /* swapcontext(&main_context, &CURRENT_THREAD->context); */
    /* swapcontext(&TMP_THREAD->context, &CURRENT_THREAD->context ); */

  }

  /* thread is done */
  else if(CURRENT_THREAD->status == DONE){
    debug("Thread: %i : done", CURRENT_THREAD->id);
    if ( g_queue_is_empty(queue) ) {
      debug("Empty queue; returning") ;
      CURRENT_THREAD = NULL;
      setcontext(&main_context);
      return 0;
    }
    CURRENT_THREAD = g_queue_pop_head(queue);
    CURRENT_THREAD->status = READY;
    /* swapcontext(&main_context, &CURRENT_THREAD->context); */
    setcontext(&main_context);

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

int wait_for_threads()
{
  AM_WAITING = 1;
  debug("Waiting for threads");
  while (g_queue_get_length(queue) >= 1){
    unmthread_yield();
    debug("Waiting for threads...");
  } 
  debug("exiting Waiting for threads...");
  AM_WAITING = 0;
  return 0;
}



int unmthread_cond_init(unmcond_t *c);
int unmthread_cond_wait(unmcond_t *c, unmmutex_t *m);
int unmthread_cond_signal(unmcond_t *c);
int unmthread_cond_uninit(unmcond_t *c);

/* initalizes the counter to zero. safe for single threads */
int unmthread_mutex_init(unmmutex_t *m){
  /* lock is zero and available */
  m->lock = 0;
}

int test_and_set(int *old_val, int new)
{
  int old = *old_val;
  *old_val = new;
  return old;
}

int unmthread_mutex_lock(unmmutex_t *m)
{
  while (test_and_set(&m->lock, 1) == 1) sched_yield();
  return 0;
}

int unmthread_mutex_unlock(unmmutex_t *m)
{
  m->lock = 0;
  return 0;
}

int unmthread_mutex_uninit(unmmutex_t *m);

