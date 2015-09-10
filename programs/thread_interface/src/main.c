#include <glib.h>
#include <stdio.h>
#include <unmthread.h>
#include <stdlib.h>
#include "dbg.h"


void spinner()
{
  int i = 0;
  for(i = 0; i < 10; i++){
    if (i == 5) unmthread_yield();
    printf("Thread %d: Spinning: %d\n", unmthread_current()->id, i);
  }
  return;
}

void exp_2(){
  int i;
  for (i = 0; i < 10; i++){
    printf("Thread: %d: 2 << %d: %d\n", unmthread_current()->id, i, 2 << i);
  }
  return;
}


int main()
{
  /* getcontext(&main_context); */
  printf("testing stuff in main\n");

  struct unmthread *p = thread_create();
  struct unmthread *c = thread_create();
  int i = unmthread_create(p, (void *) spinner, (void *) 2, NULL);
  int w = unmthread_create(c, (void *) exp_2, (void *) 2, NULL);

  return 0;

  error:
    return 1;
}

