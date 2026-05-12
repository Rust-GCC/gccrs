/* PR tree-optimization/115571 */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-optimized-raw" } */

float f(float a)
{
  int t = a < 0.5;
  return t;
}

float f0(float a)
{
  return (short)(a < 0.5);
}

/* { dg-final { scan-tree-dump-times "gimple_cond |gimple_assign <cond_expr" 2 "optimized" } } */
