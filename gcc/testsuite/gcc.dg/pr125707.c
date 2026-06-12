/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int
rshift_bit_not_int_x_x (int x)
{
  return (~x) >> x;
}

long
rshift_bit_not_long_x_x (long x)
{
  return (~x) >> x;
}

/* { dg-final { scan-tree-dump-times "return -1;" 2 "optimized" } } */
