/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define BS(X) __builtin_bitreverse16 (X)

unsigned short
foo1 (unsigned short a)
{
  return BS (~ BS (a));
}

unsigned short
foo2 (unsigned short a)
{
  return BS (BS (a) & 0xA00);
}

unsigned short
foo3 (unsigned short a)
{
  return BS (BS (a) | 0xA00);
}

unsigned short
foo4 (unsigned short a)
{
  return BS (BS (a) ^ 0xA00);
}

unsigned short
foo5 (unsigned short a, unsigned short b)
{
  return BS (BS (a) & BS (b));
}

unsigned short
foo6 (unsigned short a, unsigned short b)
{
  return BS (BS (a) | BS (b));
}

unsigned short
foo7 (unsigned short a, unsigned short b)
{
  return BS (BS (a) ^ BS (b));
}

unsigned short
foo8 (unsigned short a)
{
  return BS (BS (BS (BS (a))));
}

bool
foo9 (unsigned short a)
{
  return BS (a) == 0;
}

/* { dg-final { scan-tree-dump-not "__builtin_bitreverse" "optimized" } } */
