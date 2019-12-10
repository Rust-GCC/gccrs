/* { dg-do run } */
/* { dg-require-effective-target ia32 } */
/* { dg-require-effective-target sse2 } */
/* { dg-options "-std=gnu99 -msse2 -mstackrealign -mpreferred-stack-boundary=4" } */

#include "sse2-check.h"

#include <emmintrin.h>

__m128i __attribute__ ((__noinline__))
vector_using_function ()
{
  volatile __m128i vx;	/* We want to force a vector-aligned store into the stack.  */
  vx = _mm_xor_si128 (vx, vx);
  return vx;
}
int __attribute__ ((__noinline__))
self_aligning_function (int x, int y)
{
  __m128i ignored = vector_using_function ();
  return (x + y);
}
int g_1 = 20;
int g_2 = 22;

static void __attribute__ ((__optimize__ ("-fno-omit-frame-pointer")))
sse2_test (void)
{
  int result;
  register int reg asm ("ecx");
  asm ("pushl\t%0": : "r" (reg) : "memory"); /* Disalign runtime stack.  */
  result = self_aligning_function (g_1, g_2);
  if (result != 42)
    abort ();
  asm ("popl\t%0" : "=r" (reg));
}
