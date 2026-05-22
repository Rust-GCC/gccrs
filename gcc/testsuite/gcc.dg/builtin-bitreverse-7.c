/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define BS(X) __builtin_bitreverse8 (X)

unsigned char
foo1 (unsigned char a)
{
  return BS (~ BS (a));
}

unsigned char
foo2 (unsigned char a)
{
  return BS (BS (a) & 0x0A);
}

unsigned char
foo3 (unsigned char a)
{
  return BS (BS (a) | 0x0A);
}

unsigned char
foo4 (unsigned char a)
{
  return BS (BS (a) ^ 0x0A);
}

unsigned char
foo5 (unsigned char a, unsigned char b)
{
  return BS (BS (a) & BS (b));
}

unsigned char
foo6 (unsigned char a, unsigned char b)
{
  return BS (BS (a) | BS (b));
}

unsigned char
foo7 (unsigned char a, unsigned char b)
{
  return BS (BS (a) ^ BS (b));
}

unsigned char
foo8 (unsigned char a)
{
  return BS (BS (BS (BS (a))));
}

bool
foo9 (unsigned char a)
{
  return BS (a) == 0;
}

/* { dg-final { scan-tree-dump-not "__builtin_bitreverse" "optimized" } } */
