/* { dg-do run } */
/* { dg-options "-O2 -ffast-math -ftree-vectorize -msse4.1" } */
/* { dg-require-effective-target sse4 } */

#ifndef CHECK_H
#define CHECK_H "sse4_1-check.h"
#endif

#ifndef TEST
#define TEST sse4_1_test
#endif

#include CHECK_H

#define __NO_MATH_INLINES
#include "math_m_pi.h"

#define NUM 64

static void
__attribute__((__target__("fpmath=sse")))
init_src (float *src)
{
  int i, sign = 1;
  float f = rand ();

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
__attribute__((__target__("fpmath=387")))
TEST (void)
{
  float a[NUM];
  float r[NUM];
  int i;

  init_src (a);

  for (i = 0; i < NUM; i++)
    r[i] = __builtin_floorf (a[i]);

  /* check results:  */
  for (i = 0; i < NUM; i++)
    if (r[i] != __builtin_floorf (a[i]))
      abort();
}
