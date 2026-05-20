/* { dg-do compile { target { ! int16 } } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

/* Requires sizeof(int) > sizeof(short).  */

short
eq_short_not (int a, int b)
{
  short sa = ~a;
  short sb = ~b;
  return sa == sb;
}

short
ne_short_not (int a, int b)
{
  short sa = ~a;
  short sb = ~b;
  return sa != sb;
}

/* Verify XOR form is produced for narrowing short comparisons.  */
/* { dg-final { scan-tree-dump-times "\\^" 2 "optimized" } } */
