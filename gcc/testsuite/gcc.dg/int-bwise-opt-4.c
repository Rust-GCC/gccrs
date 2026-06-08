/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int f2(int a, int b)
{
  return (a != 0) & ((a | b) != 0);
}

/* { dg-final { scan-tree-dump-times "\&" 0 "optimized" } } */
