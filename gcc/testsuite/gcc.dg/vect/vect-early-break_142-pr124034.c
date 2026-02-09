/* { dg-add-options vect_early_break } */

#include "tree-vect.h"

__attribute__((noipa))
static 
long
memeqbyte (char byte, void *data, long length)
{
  char *p = data;
  for (long i = 0; i < 16; i++, length--)
    {
      if (length == 0)
        return 0;
      if (p[i] != byte)
        return 0;
    }
  return length;
}

#define n 16

int
main ()
{
  char iov[n];
  check_vect ();
  __builtin_memset (iov, 'x', n);
  if (memeqbyte('x', iov, n))
   __builtin_abort();
}
