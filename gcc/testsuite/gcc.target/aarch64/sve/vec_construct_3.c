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
#include <arm_sve.h>

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

void
foo (svint16_t src0, svint32_t src1, svint16_t src2, svint32_t src3,
     svint32_t src4, svint16_t src5, svint32_t src6, svint16_t src7)
{
  svbool_t all16 = svptrue_b16 ();
  svbool_t all32 = svptrue_b32 ();
  s.a = svmaxv_s16 (all16, src0);
  s.b = svminv_s32 (all32, src1);
  s.c = svlastb_s16 (svptrue_pat_b16 (SV_VL1), src2);
  s.d = svaddv_s32 (all32, src3);
  s.e = svmaxv_s32 (all32, src4);
  s.f = svminv_s16 (all16, src5);
  s.g = svlastb_s32 (svptrue_pat_b32 (SV_VL1), src6);
  s.h = svaddv_s16 (all16, src7);
}

/* { dg-final { scan-assembler-times {\tins\tv[0-9]+\.b\[[0-9]+\], v[0-9]+\.b\[[0-9]+\]\n} 7 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, } 1 } } */

/* { dg-final { scan-assembler-not {\tstr\tb[0-9]+, } } } */
/* { dg-final { scan-assembler-not {\tstrb\tw[0-9]+, } } } */
