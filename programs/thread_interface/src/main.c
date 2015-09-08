#include <glib.h>
#include <stdio.h>
#include <unmthread.h>
#include <stdlib.h>
#include "dbg.h"

int main()
{
  printf("testing stuff in main\n");
  GList *list = NULL;
list = g_list_append(list, "Hi!");
printf("The first item is '%s'\n", g_list_first(list)->data);
  struct unmthread *t = thread_create(1, 2);
  printf("%d\n", t->test);

  return 0;

  error:
      return 1;
}
