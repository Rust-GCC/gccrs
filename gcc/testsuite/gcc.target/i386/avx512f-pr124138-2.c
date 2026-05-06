/* PR target/124138 */
/* { dg-do run } */
/* { dg-options "-mavx512f" } */
/* { dg-require-effective-target avx512f } */

#define AVX512F
#include "avx512f-helper.h"

typedef __attribute__((__vector_size__ (64))) unsigned long long V;
typedef __attribute__((__vector_size__ (64))) double W;

[[gnu::noipa]] W
foo (W x)
{
  return __builtin_shuffle (x, (V) { 0, 1, 2, 5, 4, 5, 6, 7 });
}

void
TEST ()
{
  W x = foo ((W) { 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5 });
  if (x[0] != 1.5 || x[1] != 2.5 || x[2] != 3.5 || x[3] != 6.5
      || x[4] != 5.5 || x[5] != 6.5 || x[6] != 7.5 || x[7] != 8.5)
    __builtin_abort ();
}
