/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

static int int_prec = sizeof (int) * __CHAR_BIT__ - 1;

int
max_i32 (int x, int k)
{
  return x < k ? k : x;
}

int
min_i32 (int x, int k)
{
  return x > k ? k : x;
}

_Bool
x_bit_ior_rshift_x (int x)
{
  return (x | (x >> int_prec)) == max_i32 (x, -1);
}

_Bool
x_bit_ior_bit_not_rshift_x (int x)
{
  return (x | ~(x >> int_prec)) == min_i32 (x, -1);
}

_Bool
x_bit_and_rshift_x (int x)
{
  return (x & (x >> int_prec)) == min_i32 (x, 0);
}

_Bool
x_bit_and_bit_not_rshift_x (int x)
{
  return (x & ~(x >> int_prec)) == max_i32 (x, 0);
}

/* { dg-final { scan-tree-dump-times "return 1;" 4 "optimized" } } */
