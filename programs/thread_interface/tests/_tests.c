#include <stdio.h>

int main(void)
{
  int i = 0;
  i = unmthread_yield();
  printf("blah");
  return 0;
}
