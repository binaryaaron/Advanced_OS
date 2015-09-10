#define _XOPEN_SOURCE 500
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "dbg.h"

/* nessecary for apple ucontext as they are deprecated */
/* #ifdef __APPLE__ */
/* #endif */


/* stack size defined from assignment, 8KB*/
#define THREAD_STACK_SIZE (1024 * 8)

typedef enum {RUNNING, DONE} status;
typedef enum {GO, STOP} condition;

struct unmthread {
  int id;
  ucontext_t context;
  status status;
  void *stack;
  /* void *func; */
};


struct {
  condition cond;
} unmcond;

struct unmmutex {
  int lock;
};

extern int THREAD_RUNNING;
unmthread_t *CURRENT_THREAD;

/* main queue for all threads */
GQueue* queue;
static struct unmthread *main_thread;

void context_helper(unmthread_t *thread);
void print_thread(unmthread_t *thread);
unmthread_t *thread_create();

