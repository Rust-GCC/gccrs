/* { dg-do run } */
/* { dg-options "-O2 -ffast-math -ftree-vectorize -msse2" } */
/* { dg-require-effective-target sse2 } */

#ifndef CHECK_H
#define CHECK_H "sse2-check.h"
#endif

#ifndef TEST
#define TEST sse2_test
#endif

#include CHECK_H

#include "math_m_pi.h"

#define NUM 64

static void
__attribute__((__target__("fpmath=sse")))
init_src (double *src)
{
  int i, sign = 1;
  double f = rand ();

  for (i = 0; i < NUM; i++)
    {
      src[i] = (i + 1) * f * M_PI * sign;
      if (i < (NUM / 2))
	{
          if ((i % 6) == 0)
	    f = f * src[i];
        }
      else if (i == (NUM / 2))
	f = rand ();
      else if ((i % 6) == 0)
	f = 1 / (f * (i + 1) * src[i] * M_PI * sign);
      sign = -sign;
    }
}

static void
__attribute__((__target__("fpmath=sse")))
TEST (void)
{
  double a[NUM];
  float r[NUM];
  int i;

  init_src (a);

  for (i = 0; i < NUM; i++)
    r[i] = (float) a[i];

  /* check results:  */
  for (i = 0; i < NUM; i++)
    if (r[i] != (float) a[i])
      abort();
}
