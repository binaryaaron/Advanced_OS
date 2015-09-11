#ifdef __APPLE__
#define _XOPEN_SOURCE 500
#endif
#ifdef __linux__
#define _XOPEN_SOURCE 700
#endif
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sched.h>
#include "dbg.h"

/* nessecary for apple ucontext as they are deprecated */
/* #ifdef __APPLE__ */
/* #endif */


/* stack size defined from assignment, 8KB*/
#define THREAD_STACK_SIZE (1024 * 8)
#define null_thread (unmthread_t *)0

typedef enum {
    NONE,
    RUNNING,
    READY,
    BLOCKED,
    DONE
  }t_state ;

struct unmthread {
  int id;
  ucontext_t context;
  t_state status;
  /* int done; */
  void *ret_val;
  void *stack;
  void *f_args;
  void *schedule_info;
};

GQueue* queue;
unmthread_t *CURRENT_THREAD;
unmthread_t *TMP_THREAD;
unmthread_t *MAIN_THREAD;
unmthread_t *NEXT_THREAD;

int wait_for_threads();

typedef struct unmcond {
  int signal; 
} unmcond_t;

typedef struct unmmutex {
  int lock;
} unmmutex_t;

int test_and_set(int *old, int new);


