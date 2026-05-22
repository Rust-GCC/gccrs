/* { dg-do compile { target int32 } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define BS(X) __builtin_bitreverse32 (X)

unsigned
foo1 (unsigned a)
{
  return BS (~ BS (a));
}

unsigned
foo2 (unsigned a)
{
  return BS (BS (a) & 0xA0000);
}

unsigned
foo3 (unsigned a)
{
  return BS (BS (a) | 0xA0000);
}

unsigned
foo4 (unsigned a)
{
  return BS (BS (a) ^ 0xA0000);
}

unsigned
foo5 (unsigned a, unsigned b)
{
  return BS (BS (a) & BS (b));
}

unsigned
foo6 (unsigned a, unsigned b)
{
  return BS (BS (a) | BS (b));
}

unsigned
foo7 (unsigned a, unsigned b)
{
  return BS (BS (a) ^ BS (b));
}

unsigned
foo8 (unsigned a)
{
  return BS (BS (BS (BS (a))));
}

bool
foo9 (unsigned a)
{
  return BS (a) == 0;
}

/* { dg-final { scan-tree-dump-not "__builtin_bitreverse" "optimized" } } */
