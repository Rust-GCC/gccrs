/* { dg-additional-options "-fdump-tree-optimized" } */

#include "tree-vect.h"

#define N 4

int a[N];

int __attribute__ ((noipa))
f1 (void)
{
  int b[N] = { 0, 15, 15, 15 }, res = 0;
  for (int i = 0; i < N; ++i)
    res += a[i] & b[i];
  return res;
}

int __attribute__ ((noipa))
f2 (void)
{
  int b[N] = { 0, 31, 0, 31 }, res = 0;
  for (int i = 0; i < N; ++i)
    res += a[i] & b[i];
  return res;
}

int __attribute__ ((noipa))
f3 (void)
{
  int b[N] = { -1, -1, 0, 0 }, res = 0;
  for (int i = 0; i < N; ++i)
    res += a[i] & b[i];
  return res;
}

int
main ()
{
  check_vect ();

  for (int i = 0; i < N; ++i)
    a[i] = 0xe0 + i;

  if (f1 () != (a[1] & 15) + (a[2] & 15) + (a[3] & 15)
      || f2 () != (a[1] & 31) + (a[3] & 31)
      || f3 () != a[0] + a[1])
    __builtin_abort ();

  return 0;
}
