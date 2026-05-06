/* PR tree-optimization/124715 */
/* { dg-do compile } */
/* { dg-options "-O2 -fno-math-errno -fdump-tree-optimized" } */
/* { dg-require-effective-target c99_runtime } */

extern double pow(double, double);

double foo(double x)
{
  return pow(x,-1.0);
}

/* { dg-final { scan-tree-dump-not "pow " "optimized" } } */
/* { dg-final { scan-tree-dump " / " "optimized" } } */
