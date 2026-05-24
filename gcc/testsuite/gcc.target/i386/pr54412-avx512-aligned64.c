/* PR target/54412 */
/* { dg-do run { target { avx512f && { ! ia32 } } } } */
/* { dg-skip-if "PR target/54412 exercises the Win64 ABI" { ! x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx512f" } */

#include "avx512-check.h"

typedef double v8d __attribute__ ((vector_size (64), aligned (64)));

__attribute__ ((noinline)) static v8d
roundtrip_v8d (v8d x)
{
  return x;
}

static void
test_512 (void)
{
  v8d x __attribute__ ((aligned (64)))
    = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
  v8d expected __attribute__ ((aligned (64)))
    = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
  v8d r __attribute__ ((aligned (64)));

  r = roundtrip_v8d (x);

  if (__builtin_memcmp (&r, &expected, sizeof (r)) != 0)
    __builtin_abort ();
}
