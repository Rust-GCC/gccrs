/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from a horizontal
   reduction is not vectorized by constructing a vector and storing it
   if the results of the reductions are widened.  Widening typically
   causes the result of a reduction to be transferred to a GPR, therefore
   vectorization would require GPR-to-SIMD-register transfers.
 */
#include <arm_neon.h>

struct S
{
  int32_t a, b, c, d, e, f, g, h;
} s;

void
foo (int16x4_t src0, int8x8_t src1, int16x4_t src2, int8x8_t src3,
     int8x8_t src4, int16x4_t src5, int8x8_t src6, int16x4_t src7)
{
  s.a = vmaxv_s16 (src0);
  s.b = vminv_s8 (src1);
  s.c = vduph_lane_s16 (src2, 2);
  s.d = vaddv_s8 (src3);
  s.e = vmaxv_s8 (src4);
  s.f = vminv_s16 (src5);
  s.g = vdupb_lane_s8 (src6, 1);
  s.h = vaddv_s16 (src7);
}

/* { dg-final { scan-assembler-times {\tstp\tw[0-9]+, w[0-9]+,} 4 } } */

/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.s\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tfmov\ts[0-9]+, w[0-9]+\n} } }
/* { dg-final { scan-assembler-not {\tstp\tq[0-9]+, q[0-9]+,} } } */
