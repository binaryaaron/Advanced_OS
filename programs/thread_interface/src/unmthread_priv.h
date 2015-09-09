#define _XOPEN_SOURCE 500
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "dbg.h"

/* nessecary for apple ucontext as they are deprecated */
/* #ifdef __APPLE__ */
/* #endif */


/* change size of stack? */
#define THREAD_STACK_SIZE (1024 * 1024)
typedef enum {RUNNING, JOINABLE, WAITING, DONE} status;
typedef enum {GO, STOP} condition;

struct unmthread {
  int id;
  ucontext_t context;
  status status;
  void *stack;
  void *func;
};

struct {
  condition cond;
} unmcond;

struct unmmutex {
  int lock;
};


/* main queue for all threads */
static GQueue* queue;


unmthread_t *thread_create(int id, status status);

void print_thread(unmthread_t *thread);
