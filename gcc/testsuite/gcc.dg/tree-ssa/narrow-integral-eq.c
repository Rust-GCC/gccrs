/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

unsigned char
eq_uchar (unsigned int a, unsigned int b)
{
  unsigned char ua = a;
  unsigned char ub = b;
  return ua == ub;
}

signed char
ne_schar (int a, int b)
{
  signed char ca = a;
  signed char cb = b;
  return ca != cb;
}

int
eq_int_from_ll (long long a, long long b)
{
  int ia = a;
  int ib = b;
  return ia == ib;
}

int
eq_int_no_narrow (int a, int b)
{
  int ia = a;
  int ib = b;
  return ia == ib;
}

long long
eq_widen (int a, int b)
{
  long long la = a;
  long long lb = b;
  return la == lb;
}

/* Only narrowing integral cases should produce XOR form.
   eq_int_no_narrow and eq_widen should not match.  */
/* { dg-final { scan-tree-dump-times "\\^" 3 "optimized" } } */
