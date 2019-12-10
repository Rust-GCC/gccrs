/* { dg-do run } */
/* { dg-options "-O2 -mavx512dq" } */
/* { dg-require-effective-target avx512dq } */

#define AVX512DQ
#include "avx512f-helper.h"

#include <math.h>
#include <limits.h>
#include <float.h>
#define SIZE (128 / 64)
#include "avx512f-mask-type.h"

#ifndef __FPCLASSSD__
#define __FPCLASSSD__
int check_fp_class_dp (double src, int imm)
{
  int qNaN_res = isnan (src);
  int sNaN_res = isnan (src);
  int Pzero_res = (src == 0.0);
  int Nzero_res = (src == -0.0);
  int PInf_res = (isinf (src) == 1);
  int NInf_res = (isinf (src) == -1);
  int Denorm_res = (fpclassify (src) == FP_SUBNORMAL);
  int FinNeg_res = __builtin_finite (src) && (src < 0);

  int result = (((imm & 1) && qNaN_res)
		|| (((imm >> 1) & 1) && Pzero_res)
		|| (((imm >> 2) & 1) && Nzero_res)
		|| (((imm >> 3) & 1) && PInf_res)
		|| (((imm >> 4) & 1) && NInf_res)
		|| (((imm >> 5) & 1) && Denorm_res)
		|| (((imm >> 6) & 1) && FinNeg_res)
		|| (((imm >> 7) & 1) && sNaN_res));
  return result;
}
#endif

__mmask8
CALC (double *s1, int imm)
{
  int i;
  __mmask8 res = 0;

  if (check_fp_class_dp(s1[0], imm))
    res = res | 1;

  return res;
}

void
TEST (void)
{
  int i;
  union128d src;
  __mmask8 res1, res2, res_ref = 0;
  __mmask8 mask = MASK_VALUE;

  src.a[0] = 1.0 / 0.0;
  for (i = 1; i < SIZE; i++)
    {
      src.a[i] = -24.43 + 0.6 * i;
    }

  res1 = _mm_fpclass_sd_mask (src.x, 0xFF);
  res2 = _mm_mask_fpclass_sd_mask (mask, src.x, 0xFF);

  res_ref = CALC (src.a, 0xFF);

  if (res_ref != res1)
    abort ();

  if ((res_ref & mask) != res2)
    abort ();
}
