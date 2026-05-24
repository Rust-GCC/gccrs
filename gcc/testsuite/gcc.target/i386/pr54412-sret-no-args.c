/* PR target/54412 */
/* { dg-do run { target { avx && { ! ia32 } } } } */
/* { dg-skip-if "PR target/54412 exercises the Win64 ABI" { ! x86_64-*-mingw* } } */
/* { dg-options "-O2 -mavx -std=gnu99" } */

#include "avx-check.h"
#include <immintrin.h>

struct avx_wrapper
{
  __m256 value;
};

__attribute__ ((noinline)) static struct avx_wrapper
make_wrapper (void)
{
  struct avx_wrapper x;

  x.value = _mm256_set_ps (8.0f, 7.0f, 6.0f, 5.0f,
                           4.0f, 3.0f, 2.0f, 1.0f);
  return x;
}

static void
avx_test (void)
{
  struct avx_wrapper expected;
  struct avx_wrapper r;

  expected.value = _mm256_set_ps (8.0f, 7.0f, 6.0f, 5.0f,
                                  4.0f, 3.0f, 2.0f, 1.0f);
  r = make_wrapper ();

  if (__builtin_memcmp (&r, &expected, sizeof (r)) != 0)
    __builtin_abort ();
}
