/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define BS(X) __builtin_bitreverse64 (X)

unsigned long long
foo1 (unsigned long long a)
{
  return BS (~ BS (a));
}

unsigned long long
foo2 (unsigned long long a)
{
  return BS (BS (a) & 0xA0000);
}

unsigned long long
foo3 (unsigned long long a)
{
  return BS (BS (a) | 0xA0000);
}

unsigned long long
foo4 (unsigned long long a)
{
  return BS (BS (a) ^ 0xA0000);
}

unsigned long long
foo5 (unsigned long long a, unsigned long long b)
{
  return BS (BS (a) & BS (b));
}

unsigned long long
foo6 (unsigned long long a, unsigned long long b)
{
  return BS (BS (a) | BS (b));
}

unsigned long long
foo7 (unsigned long long a, unsigned long long b)
{
  return BS (BS (a) ^ BS (b));
}

unsigned long long
foo8 (unsigned long long a)
{
  return BS (BS (BS (BS (a))));
}

bool
foo9 (unsigned long long a)
{
  return BS (a) == 0;
}

/* { dg-final { scan-tree-dump-not "__builtin_bitreverse" "optimized" } } */
