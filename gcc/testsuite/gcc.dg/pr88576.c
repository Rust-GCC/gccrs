/* { dg-do compile } */
/* { dg-options "-O2 -fno-math-errno -fdump-tree-optimized" } */

#include <errno.h>
#include <stdlib.h>

char *foo(unsigned long size)
{
  errno = 0;
  char *p = malloc (size);
  if (errno != 0)
    abort ();
  return p;
}

/* { dg-final { scan-tree-dump "abort" "optimized" } } */
