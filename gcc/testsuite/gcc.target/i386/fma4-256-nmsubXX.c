/* { dg-do run } */
/* { dg-require-effective-target fma4 } */
/* { dg-options "-std=gnu17 -O2 -mfma4" } */

#include "fma4-check.h"

#include <x86intrin.h>
#include <string.h>

#define NUM 20

union
{
  __m256 x[NUM];
  float f[NUM * 8];
  __m256d y[NUM];
  double d[NUM * 4];
} dst, res, src1, src2, src3;

/* Note that in macc*,msub*,mnmacc* and mnsub* instructions, the intermdediate 
   product is not rounded, only the addition is rounded. */

static void
init_nmsubps ()
{
  int i;
  for (i = 0; i < NUM * 8; i++)
    {
      src1.f[i] = i;
      src2.f[i] = i + 10;
      src3.f[i] = i + 20;
    }
}

static void
init_nmsubpd ()
{
  int i;
  for (i = 0; i < NUM * 4; i++)
    {
      src1.d[i] = i;
      src2.d[i] = i + 10;
      src3.d[i] = i + 20;
    }
}

static int
check_nmsubps ()
{
  int i, j, check_fails = 0;
  for (i = 0; i < NUM * 8; i = i + 8)
    for (j = 0; j < 8; j++)
      {
	res.f[i + j] = - (src1.f[i + j] * src2.f[i + j]) - src3.f[i + j];
	if (dst.f[i + j] != res.f[i + j]) 
	  check_fails++;
      }
  return check_fails;
}

static int
check_nmsubpd ()
{
  int i, j, check_fails = 0;
  for (i = 0; i < NUM * 4; i = i + 4)
    for (j = 0; j < 4; j++)
      {
	res.d[i + j] = - (src1.d[i + j] * src2.d[i + j]) - src3.d[i + j];
	if (dst.d[i + j] != res.d[i + j]) 
	  check_fails++;
      }
  return check_fails;
}

static void
fma4_test (void)
{
  int i;

  init_nmsubps ();
  
  for (i = 0; i < NUM; i++)
    dst.x[i] = _mm256_nmsub_ps (src1.x[i], src2.x[i], src3.x[i]);
  
  if (check_nmsubps (&dst.x[i], &src1.f[i * 4], &src2.f[i * 4], &src3.f[i * 4])) 
    abort ();

  init_nmsubpd ();
  
  for (i = 0; i < NUM; i++)
    dst.y[i] = _mm256_nmsub_pd (src1.y[i], src2.y[i], src3.y[i]);
  
  if (check_nmsubpd (&dst.y[i], &src1.d[i * 2], &src2.d[i * 2], &src3.d[i * 2])) 
    abort ();
}
