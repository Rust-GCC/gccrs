/* PR middle-end/124630 */
/* { dg-do compile } */
/* { dg-require-effective-target ctz } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

unsigned
foo (unsigned a)
{
  if (a != 0)
    return __builtin_popcount (a ^ (a - 1)) - 1;
  else
    return __CHAR_BIT__ * __SIZEOF_INT__;
}

/* { dg-final { scan-tree-dump-times "__builtin_ctz|\\.CTZ" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-not "__builtin_popcount|\\.POPCOUNT" "optimized" } } */
