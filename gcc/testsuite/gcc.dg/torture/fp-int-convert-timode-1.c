/* Test for correct rounding of conversions from __int128 to
   float.  */
/* { dg-do run } */
/* { dg-require-effective-target int128 } */
/* { dg-options "-frounding-math" } */

#include <stdlib.h>

int
main (void)
{
  volatile unsigned long long h = 0x8000000000000000LL;
  volatile unsigned long long l = 0xdLL;
  volatile unsigned __int128 u128 = (((unsigned __int128) h) << 64) | l;
  volatile __int128 s128 = u128;
  float fs = s128;
  if (fs != -0x1p+127)
    abort ();
  double ds = s128;
  if (ds != -0x1p+127)
    abort ();
  exit (0);
}
