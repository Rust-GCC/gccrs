/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from a horizontal
   reduction is vectorized by constructing a vector and storing it
   even if the results of the reductions are narrowed.
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
foo (int16x4_t src0, int32x2_t src1, int16x4_t src2, int32x2_t src3,
     int32x2_t src4, int16x4_t src5, int32x2_t src6, int16x4_t src7)
{
  s.a = vmaxv_s16 (src0);
  s.b = vminv_s32 (src1);
  s.c = vduph_lane_s16 (src2, 2);
  s.d = vaddv_s32 (src3);
  s.e = vmaxv_s32 (src4);
  s.f = vminv_s16 (src5);
  s.g = vdups_lane_s32 (src6, 1);
  s.h = vaddv_s16 (src7);
}

/* { dg-final { scan-assembler-times {\tins\tv[0-9]+\.b\[[0-9]+\], v[0-9]+\.b\[[0-9]+\]\n} 7 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, } 1 } } */

/* { dg-final { scan-assembler-not {\tstr\tb[0-9]+, } } } */
/* { dg-final { scan-assembler-not {\tstrb\tw[0-9]+, } } } */
