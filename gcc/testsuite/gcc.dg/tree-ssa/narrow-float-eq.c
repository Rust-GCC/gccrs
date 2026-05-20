/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int
eq_float_no_xor (double a, double b)
{
  float fa = a;
  float fb = b;
  return fa == fb;
}

/* Floating-point narrowing should not use XOR form.  */
/* { dg-final { scan-tree-dump-not "\\^" "optimized" } } */
