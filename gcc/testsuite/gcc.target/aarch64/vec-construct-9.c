/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from a horizontal
   reduction is vectorized by constructing a vector and storing it.
   Since there are no GPR-to-SIMD register transfers, there is no
   need to charge additional costs for them.  Only one STR (Store
   vector reg, unsigned immed, B/H/S/D-form) instruction is required
   instead of 8.
 */
#include <arm_neon.h>

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

void
foo (int8x8_t src0, int8x8_t src1, int8x8_t src2, int8x8_t src3, int8x8_t src4,
     int8x8_t src5, int8x8_t src6, int8x8_t src7)
{
  s.a = vmaxv_s8 (src0);
  s.b = vminv_s8 (src1);
  s.c = vdupb_lane_s8 (src2, 2);
  s.d = vaddv_s8 (src3);
  s.e = vmaxv_s8 (src4);
  s.f = vminv_s8 (src5);
  s.g = vdupb_lane_s8 (src6, 1);
  s.h = vaddv_s8 (src7);
}

/* { dg-final { scan-assembler-times {\tins\tv[0-9]+\.b\[[0-9]+\], v[0-9]+\.b\[[0-9]+\]\n} 7 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, } 1 } } */

/* { dg-final { scan-assembler-not {\tstr\tb[0-9]+, } } } */
