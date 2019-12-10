/* { dg-do compile } */
/* { dg-options "-O2" } */

#include <arm_sve.h>

void
test1 (int32_t x, int32_t y, int *any, svbool_t *ptr)
{
  svbool_t res = svwhilele_b16 (x, y);
  *any = svptest_last (svptrue_b16 (), res);
  *ptr = res;
}

int
test2 (int32_t x, int32_t y)
{
  svbool_t res = svwhilele_b16 (x, y);
  return svptest_last (svptrue_b16 (), res);
}

/* { dg-final { scan-assembler-times {\twhilele\t} 2 } } */
/* { dg-final { scan-assembler-not {\tptrue\t} } } */
/* { dg-final { scan-assembler-not {\tptest\t} } } */
