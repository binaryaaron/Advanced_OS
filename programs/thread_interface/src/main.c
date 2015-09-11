#include <glib.h>
#include <stdio.h>
#include <unmthread.h>
#include <stdlib.h>
#include "dbg.h"


void spinner()
{
  debug("in spinner");
  int i = 0;
  /* for(;;); */
  for(i = 0; i < 5; i++){
    /* if (i == 5) unmthread_yield(); */
    printf("Thread %d: Spinning: %d\n", unmthread_current()->id, i);
  }
  return;
}

void exp_2(){
  debug("in exp2");
  int i = 0;
  for (i = 0; i < 5; i++){
    /* if (i % 2 == 0) unmthread_yield(); */
    printf("Thread: %d: 2 << %d: %d\n", unmthread_current()->id, i, 2 << i);
  }
  return;
}


int main()
{
  printf("testing stuff in main\n");

  struct unmthread *p; //h= thread_create();
  struct unmthread *c; //= thread_create();
  int i = unmthread_create(p, (void *) exp_2, NULL, NULL);
  debug("created thread 1");
  int w = unmthread_create(c, (void *) spinner, NULL, NULL);
  debug("created thread 2");

  debug ("exiting");
  return 0;

}

