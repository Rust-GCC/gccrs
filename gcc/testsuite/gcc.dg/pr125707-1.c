/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

long long
rshift_bit_not_long_x_x (long long x)
{
  return (~x) >> x;
}

/* xfailed because gimplifier introduces a truncation.  */
/* { dg-final { scan-tree-dump-times "return -1;" 1 "optimized" { xfail *-*-* } } } */
