/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9-a -msve-vector-bits=256" } */

typedef __attribute__((__vector_size__(sizeof(int)*8))) signed int v8i;
typedef __attribute__((__vector_size__(sizeof(int)*8))) unsigned int v8u;

void g(v8i *a, v8i *b, v8u *c)
{
  *c = *c - (v8u)(*a * *b);
}

v8u g_(v8i a, v8i b, v8u c)
{
  return c - (v8u)(a * b);
}

void h(v8u *a, v8u *b, v8i *c)
{
  *c = *c - (v8i)(*a * *b);
}

v8i h_(v8u a, v8u b, v8i c)
{
  return c - (v8i)(a * b);
}

void x(v8u *a, v8u *b, v8i *c)
{
  *c = *c + ((v8i)(-*a) * (v8i)*b);
}

v8i x_(v8u a, v8u b, v8i c)
{
  return c + ((v8i)(-a) * (v8i)b);
}

void y(v8u *a, v8i *b,v8i *c)
{
  *c = *c + ((v8i)(-*a) * *b);
}

v8i y_(v8u a, v8i b, v8i c)
{
  return c + ((v8i)(-a) * b);
}

void z(v8i *a, v8u *b, v8u *c)
{
  *c = *c + ((v8u)(-*a) * *b);
}

v8u z_(v8i a, v8u b, v8u c)
{
  return c + ((v8u)(-a) * b);
}

/* { dg-final { scan-assembler-times "\\tmsb\\t" 5 } } */
/* { dg-final { scan-assembler-times "\\tmls\\t" 5 } } */
/* { dg-final { scan-assembler-not "\\tneg\\t" } } */
