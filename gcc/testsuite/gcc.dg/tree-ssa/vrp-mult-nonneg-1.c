/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized -fdump-tree-evrp-details" } */
/* PR tree-optimization/23471 */

int f(int a, int b)
{
  if (a == b)
  {
    int t = a*b;
    return t < 0; /* Should be optimized to 0. */
  }
  return 0;
}


/* { dg-final { scan-tree-dump-not "if " "optimized" } } */
/* { dg-final { scan-tree-dump "Global Exported: t_" "evrp" } } */
