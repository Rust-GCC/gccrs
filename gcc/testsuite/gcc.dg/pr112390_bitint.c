/* PR tree-optimization/112390 - _BitInt variants */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int fbit11_ior(unsigned _BitInt(11) a)
{
  signed _BitInt(11) sa = (signed _BitInt(11))a;
  return (sa < 0) | ((a & 1023wb) == 0);
}

int fbit11_and(unsigned _BitInt(11) a)
{
  signed _BitInt(11) sa = (signed _BitInt(11))a;
  return (sa >= 0) & ((a & 1023wb) != 0);
}

/* { dg-final { scan-tree-dump-not "bit_and" "optimized" } } */
/* { dg-final { scan-tree-dump-not "bit_ior" "optimized" } } */
/* { dg-final { scan-tree-dump-times "> 0" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "<= 0" 1 "optimized" } } */
