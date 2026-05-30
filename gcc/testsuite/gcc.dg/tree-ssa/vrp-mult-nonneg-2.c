/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-forwprop -fdump-tree-optimized -fdump-tree-evrp-details" } */
/* PR tree-optimization/125513 */


int f(int a)
{
  int t = a*a;
  return t < 0; /* Should be optimized to 0. */
}


/* { dg-final { scan-tree-dump "return 0" "optimized" } } */
/* { dg-final { scan-tree-dump "Global Exported: t_" "evrp" } } */
