/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from a horizontal
   reduction is not vectorized by constructing a vector and storing it
   if the results of the reductions are widened.  Widening typically
   causes the result of a reduction to be transferred to a GPR, therefore
   vectorization would require GPR-to-SIMD-register transfers.
 */
#include <arm_sve.h>

struct S
{
  int32_t a, b, c, d, e, f, g, h;
} s;

void
foo (svint16_t src0, svint8_t src1, svint16_t src2, svint8_t src3,
     svint8_t src4, svint16_t src5, svint8_t src6, svint16_t src7)
{
  svbool_t all16 = svptrue_b16 ();
  svbool_t all8 = svptrue_b8 ();
  s.a = svmaxv_s16 (all16, src0);
  s.b = svminv_s8 (all8, src1);
  s.c = svlastb_s16 (svptrue_pat_b16 (SV_VL1), src2);
  s.d = svaddv_s8 (all8, src3);
  s.e = svmaxv_s8 (all8, src4);
  s.f = svminv_s16 (all16, src5);
  s.g = svlastb_s8 (svptrue_pat_b8 (SV_VL1), src6);
  s.h = svaddv_s16 (all16, src7);
}

/* { dg-final { scan-assembler-times {\tstp\tw[0-9]+, w[0-9]+,} 4 } } */

/* { dg-final { scan-assembler-not {\tins\tv[0-9]+\.s\[[0-9]+\], w[0-9]+\n} } } */
/* { dg-final { scan-assembler-not {\tfmov\ts[0-9]+, w[0-9]+\n} } }
/* { dg-final { scan-assembler-not {\tstp\tq[0-9]+, q[0-9]+,} } } */
