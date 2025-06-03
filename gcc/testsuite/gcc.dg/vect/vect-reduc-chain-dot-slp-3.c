/* Disabling epilogues until we find a better way to deal with scans.  */
/* { dg-additional-options "--param vect-epilogues-nomask=0" } */
/* { dg-require-effective-target vect_int } */
/* { dg-require-effective-target arm_v8_2a_dotprod_neon_hw { target { aarch64*-*-* || arm*-*-* } } } */
/* { dg-add-options arm_v8_2a_dotprod_neon }  */

#include "tree-vect.h"

#ifndef SIGNEDNESS_1
#define SIGNEDNESS_1 signed
#define SIGNEDNESS_2 signed
#endif

SIGNEDNESS_1 int __attribute__ ((noipa))
f (SIGNEDNESS_1 int res,
   SIGNEDNESS_2 short *a,
   SIGNEDNESS_2 short *b,
   int step, int n)
{
  for (int i = 0; i < n; i++)
    {
      res += a[0] * b[0];
      res += a[1] * b[1];
      res += a[2] * b[2];
      res += a[3] * b[3];
      res += a[4] * b[4];
      res += a[5] * b[5];
      res += a[6] * b[6];
      res += a[7] * b[7];

      a += step;
      b += step;
    }

  return res;
}

#define BASE ((SIGNEDNESS_2 int) -1 < 0 ? -1026 : 373)
#define OFFSET 20

int
main (void)
{
  check_vect ();

  SIGNEDNESS_2 short a[100], b[100];
  int expected = 0x12345;
  int step = 8;
  int n = 2;
  int t = 0;

#pragma GCC novector
  for (int i = 0; i < sizeof (a) / sizeof (a[0]); ++i)
    {
      a[i] = BASE + i * 5;
      b[i] = BASE + OFFSET + i * 4;
    }

#pragma GCC novector
  for (int i = 0; i < n; i++)
    {
      expected += a[t + 0] * b[t + 0];
      expected += a[t + 1] * b[t + 1];
      expected += a[t + 2] * b[t + 2];
      expected += a[t + 3] * b[t + 3];
      expected += a[t + 4] * b[t + 4];
      expected += a[t + 5] * b[t + 5];
      expected += a[t + 6] * b[t + 6];
      expected += a[t + 7] * b[t + 7];
      t += step;
    }

  if (f (0x12345, a, b, step, n) != expected)
    __builtin_abort ();
}

/* { dg-final { scan-tree-dump "vect_recog_dot_prod_pattern: detected" "vect" } } */
/* { dg-final { scan-tree-dump "vectorizing SLP node starting from: \\S+ = DOT_PROD_EXPR" "vect" { target vect_sdot_hi } } } */
