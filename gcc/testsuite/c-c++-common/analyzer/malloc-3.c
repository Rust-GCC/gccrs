/* { dg-skip-if "requires hosted libstdc++ for stdlib malloc" { ! hostedlib } } */

#include <stdlib.h>

/* Don't complain about leaks due to exiting from "main".  */

int main (void)
{
  void *p = malloc (1024);
  return 0;
}
