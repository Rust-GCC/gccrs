/* { dg-do compile { target { int128 && lp64 } } } */
/* { dg-require-effective-target pie } */
/* { dg-options "-Os -fno-omit-frame-pointer -fPIE -fcondition-coverage -fstrub=all -mcmodel=large" } */

int a, b, c, d, e;

void
foo ()
{
  c = (__uint128_t) b >> 4;
  if (__builtin_expect (e, 1))
    if (c)
      d -= a;
}
