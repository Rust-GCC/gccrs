/* PR target/54412 */
/* { dg-do run { target { avx && { ! ia32 } } } } */
/* { dg-skip-if "PR target/54412 exercises the Win64 ABI" { ! x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */

#include "avx-check.h"

typedef float v8f __attribute__ ((vector_size (32), aligned (32)));

static const v8f expected = { 1.0f, 2.0f, 3.0f, 4.0f,
                              5.0f, 6.0f, 7.0f, 8.0f };

__attribute__ ((noinline)) static int
param_matches (v8f x)
{
  return __builtin_memcmp (&x, &expected, sizeof (x)) == 0;
}

static void
avx_test (void)
{
  v8f x __attribute__ ((aligned (32)))
    = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };

  if (!param_matches (x))
    __builtin_abort ();
}
