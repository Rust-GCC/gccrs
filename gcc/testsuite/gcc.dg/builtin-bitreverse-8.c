/* { dg-do compile { target int128 } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define BS(X) __builtin_bitreverse128 (X)

unsigned __int128
foo1 (unsigned __int128 a)
{
  return BS (~ BS (a));
}

unsigned __int128
foo2 (unsigned __int128 a)
{
  return BS (BS (a) & 0xA0000A0000ULL);
}

unsigned __int128
foo3 (unsigned __int128 a)
{
  return BS (BS (a) | 0xA0000A0000ULL);
}

unsigned __int128
foo4 (unsigned __int128 a)
{
  return BS (BS (a) ^ 0xA0000A0000ULL);
}

unsigned __int128
foo5 (unsigned __int128 a, unsigned __int128 b)
{
  return BS (BS (a) & BS (b));
}

unsigned __int128
foo6 (unsigned __int128 a, unsigned __int128 b)
{
  return BS (BS (a) | BS (b));
}

unsigned __int128
foo7 (unsigned __int128 a, unsigned __int128 b)
{
  return BS (BS (a) ^ BS (b));
}

unsigned __int128
foo8 (unsigned __int128 a)
{
  return BS (BS (BS (BS (a))));
}

bool
foo9 (unsigned __int128 a)
{
  return BS (a) == 0;
}

/* { dg-final { scan-tree-dump-not "__builtin_bitreverse" "optimized" } } */
