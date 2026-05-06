#include "tree-vect.h"

static void __attribute__((noipa))
negate (bool * x, unsigned int n)
{
  for (unsigned int i = 0; i < n; i++)
    x[i] = ! x[i];
}

int main()
{
  check_vect ();
  bool x[128], res[128];
#pragma GCC novector
  for (int i = 0; i < 128; ++i)
    {
      x[i] = i % 3 == 0;
      res[i] = !x[i];
    }
  negate (x, 128);
  if (__builtin_memcmp (x, res, 128) != 0)
    abort ();
  return 0;
}

/* { dg-final { scan-tree-dump "optimized: loop vectorized" "vect" } } */
