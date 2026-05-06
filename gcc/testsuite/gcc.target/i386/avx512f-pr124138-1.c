/* PR target/124138 */
/* { dg-do run } */
/* { dg-options "-mavx512f" } */
/* { dg-require-effective-target avx512f } */

#define AVX512F
#include "avx512f-helper.h"

typedef __attribute__((__vector_size__ (64))) unsigned long long V;

[[gnu::noipa]] V
foo (V x)
{
  return __builtin_shuffle (x, (V) { 0, 1, 2, 5, 4, 5, 6, 7 });
}

void
TEST ()
{
  V x = foo ((V) { 1, 2, 3, 4, 5, 6, 7, 8 });
  if (x[0] != 1 || x[1] != 2 || x[2] != 3 || x[3] != 6
      || x[4] != 5 || x[5] != 6 || x[6] != 7 || x[7] != 8)
    __builtin_abort ();
}
