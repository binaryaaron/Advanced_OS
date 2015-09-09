#include <glib.h>
#include <stdio.h>
#include <unmthread.h>
#include <stdlib.h>
#include "dbg.h"


void spinner()
{
  int i = 0;
  for(i; i < 10; i++){
    printf("Spinning: %d", i);
  }
}


int main()
{
  printf("testing stuff in main\n");
  thread_status s = RUNNING;
  thread_status j = JOINABLE;

  struct unmthread *t = thread_create(1, s);
  int i = unmthread_create(t, spinner, (void *) 2, (void *) s);
  print_thread(t);
  printf("%d", i);
  print_thread(unmthread_current);

  /* printf("Is the queue empty?  %s, adding folks\n",  g_queue_is_empty(q) ? "Yes" : "No"); */


  return 0;

  error:
    return 1;
}

