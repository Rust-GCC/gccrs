/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target vect_masked_store } */

/* { dg-additional-options "-mavx2" { target avx2 } } */
/* { dg-additional-options "-march=armv9-a" { target aarch64-*-* } } */

#include <stdbool.h>

void foo3 (float *restrict a, int *restrict b, int *restrict c, int n, int stride)
{
  if (stride <= 1)
    return;

  bool ai = a[0];

  for (int i = 0; i < n; i++)
    {
      int res = c[i];
      int t = b[i+stride];
      if (ai)
        t = res;
      c[i] = t;
    }
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-tree-dump-not "VEC_COND_EXPR " "vect" { target aarch64-*-* } } } */
