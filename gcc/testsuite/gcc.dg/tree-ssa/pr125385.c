/* PR tree-optimization/125385 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int
f_lt (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a < (float) b;
}

int
f_le (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a <= (float) b;
}

int
f_eq (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a == (float) b;
}

int
f_ne (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a != (float) b;
}

int
f_ge (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a >= (float) b;
}

int
f_gt (int a, int b)
{
  a &= 0xff;
  b &= 0xff;
  return (float) a > (float) b;
}

int
f_signed_range (int a, int b)
{
  a = (a & 0xff) - 128;
  b = (b & 0xff) - 128;
  return (float) a < (float) b;
}

int
f_cst (int a)
{
  a &= 0xff;
  return (float) a < 42.0f;
}

int
f_cst_fractional (int a)
{
  a &= 0xff;
  return (float) a < 42.5f;
}

int
f_cst_outside_type (int a)
{
  a &= 0xff;
  return (float) a < 2147483648.0f;
}

/* { dg-final { scan-tree-dump-not "\\(float\\)" "optimized" } } */
