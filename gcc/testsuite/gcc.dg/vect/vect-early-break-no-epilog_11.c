/* { dg-add-options vect_early_break } */
/* { dg-do run } */
/* { dg-require-effective-target vect_early_break_hw } */
/* { dg-require-effective-target vect_long } */

/* { dg-additional-options "-mavx512f" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-require-effective-target avx512f_hw { target i?86-*-* x86_64-*-* } } */

#include "tree-vect.h"

__attribute__ ((noipa))
int f1 (const unsigned long *restrict a, const unsigned long *b, int n)
{
  for (int i = 0; i < n; ++i)
    {
      if (a[i] < b[i])
	return 0;
      if (a[i] > b[i])
	return 1;
    }
  return 1;
}

__attribute__ ((noipa))
int f2 (const unsigned long *restrict a, const unsigned long *b, int n)
{
  for (int i = 0; i < n; ++i)
    {
      if (a[i] < b[i])
	return 1;
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}

int main (void)
{
  check_vect ();

  static unsigned long a[3] __attribute__ ((aligned (16))) = {10, 1, 0};
  static unsigned long b[3] __attribute__ ((aligned (16))) = {9, 2, 0};

  if (f1 (a, b, 3) != 1)
    __builtin_abort ();

  if (f2 (a, b, 3) != 1)
    __builtin_abort ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 2 "vect" } } */
/* { dg-final { scan-tree-dump-not "early break does not require epilog" "vect" } } */
