/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from the results of calls
   to a function that has only vector parameters and returns a scalar result is
   not vectorized by constructing a vector and storing it, given that the
   GPR-to-SIMD version of INS (which would have had to be used to vectorize this
   code) typically has higher latency and lower throughput than the SIMD-to-SIMD
   version of INS.  This is a test for misidentification of builtin reductions.
 */

#include <arm_neon.h>

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

int8_t __attribute__ ((noinline, const))
bar (int8x8_t v)
{
        return v[0];
}

void
foo (int8x8_t src0, int8x8_t src1, int8x8_t src2, int8x8_t src3, int8x8_t src4,
     int8x8_t src5, int8x8_t src6, int8x8_t src7)
{
  s.a = bar (src0);
  s.b = bar (src1);
  s.c = bar (src2);
  s.d = bar (src3);
  s.e = bar (src4);
  s.f = bar (src5);
  s.g = bar (src6);
  s.h = bar (src7);
}

/* { dg-final { scan-assembler-times {\tstrb\tw[0-9]+, } 8 } } */

/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.b\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tstr\td[0-9]+, } } } */
