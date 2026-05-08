/* { dg-do compile } */
/* { dg-options "-O2 -ftree-slp-vectorize" } */

/* Test that a group of stores of 8 elements derived from lane extractions is
   vectorized by constructing a vector and storing it.  Since there are no
   GPR-to-SIMD register transfers, there is no need to charge additional costs
   for them.  Only one STR (Store vector reg, unsigned immed, B/H/S/D-form)
   instruction is required instead of 8.  This is a test for over-restriction
   of an exemption from usual GPR-to-SIMD costs to reductions (in case lane
   extraction is not treated as a reduction).
 */
#include <arm_sve.h>

struct S
{
  char a, b, c, d, e, f, g, h;
} s;

void
foo (svint8_t src0, svint8_t src1, svint8_t src2, svint8_t src3, svint8_t src4,
     svint8_t src5, svint8_t src6, svint8_t src7, svbool_t p)
{
  s.a = svlastb_s8 (p, src0);
  s.b = svlastb_s8 (p, src1);
  s.c = svlastb_s8 (p, src2);
  s.d = svlastb_s8 (p, src3);
  s.e = svlastb_s8 (p, src4);
  s.f = svlastb_s8 (p, src5);
  s.g = svlastb_s8 (p, src6);
  s.h = svlastb_s8 (p, src7);
}

/* { dg-final { scan-assembler-times {\tins\tv[0-9]+\.b\[[0-9]+\], v[0-9]+\.b\[[0-9]+\]\n} 7 } } */
/* { dg-final { scan-assembler-times {\tstr\td[0-9]+, } 1 } } */

/* { dg-final { scan-assembler-not {\tstr\tb[0-9]+, } } } */
/* { dg-final { scan-assembler-not {\tstrb\tw[0-9]+, } } } */
