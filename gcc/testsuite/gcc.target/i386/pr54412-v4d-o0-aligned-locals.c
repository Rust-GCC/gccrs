/* PR target/54412 */
/* { dg-do run { target { avx && { ! ia32 } } } } */
/* { dg-skip-if "PR target/54412 exercises the Win64 ABI" { ! x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */

#include "avx-check.h"

typedef double v4d __attribute__ ((vector_size (32)));

__attribute__ ((noinline)) static v4d
f (v4d x)
{
  return x;
}

static void
avx_test (void)
{
  v4d x __attribute__ ((aligned (32))) = { 1.0, 2.0, 3.0, 4.0 };
  v4d expected __attribute__ ((aligned (32))) = { 1.0, 2.0, 3.0, 4.0 };
  v4d r __attribute__ ((aligned (32)));

  r = f (x);

  if (__builtin_memcmp (&r, &expected, sizeof (r)) != 0)
    __builtin_abort ();
}
