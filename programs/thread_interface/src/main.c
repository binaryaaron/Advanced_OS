#include <glib.h>
#include <stdio.h>
#include <unmthread.h>
#include <stdlib.h>
#include "dbg.h"


unmmutex_t the_mutex;
unmcond_t condc, condp;

void spinner()
{
  /* debug("in spinner"); */
  int i = 0;
  /* for(;;); */
  /* unmthread_mutex_lock(&the_mutex); */
  for(i = 0; i < 10; i++){
    if (i % 2 == 0) unmthread_yield();
    /* if (i == 3) unmthread_yield(); */
    debug("Thread %d: Spinning: %d", unmthread_current()->id, i);
  /* unmthread_mutex_unlock(&the_mutex); */
  }
  return;
}

void exp_2(){
  /* debug("in exp2"); */
  int i = 0;
  for (i = 0; i < 50; i++){
    /* if (i == 3) unmthread_yield(); */
    if (i % 2 == 0) unmthread_yield();
    debug("Thread: %d: 2 << %d: %d", unmthread_current()->id, i, 2 << i);
  }
  return;
}


int main()
{
  struct unmthread *a; //h= thread_create();
  struct unmthread *b; //= thread_create();
  struct unmthread *c; //= thread_create();
  /* struct unmthread *c; //= thread_create(); */
  /* struct unmthread *d; //= thread_create(); */
  unmthread_mutex_init(&the_mutex);

  unmthread_create(a, (void *) spinner, NULL, NULL);
  debug("created thread 1");
  unmthread_create(b, (void *) spinner, NULL, NULL);
  debug("created thread 2");
  /* unmthread_create(c, (void *) spinner, NULL, NULL); */
  /* debug("created thread 3"); */
  /* unmthread_create(c, (void *) exp_2, NULL, NULL); */
  /* debug("created thread 3"); */
  /* unmthread_create(d, (void *) exp_2, NULL, NULL); */


  /* unmthread_yield(); */
  int rval = 0;
  unmthread_join(&a, (void *) rval);
  unmthread_join(&b, (void *) rval);
  debug ("exiting");

  return 0;

}

