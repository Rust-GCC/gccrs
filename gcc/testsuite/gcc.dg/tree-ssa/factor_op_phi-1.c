/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt1-details -fdump-tree-optimized" } */

/* PR tree-optimization/112418 */

int f(int a, int b, int d)
{
  int c;
  if (a < 0)
  {
        a = -a;
        c = d > 0 ? d : -d;
  }
  else
  {
        a = a;
        c = d > 0 ? d : -d;
  }
  return a + c;
}

/* ABS <d> should be able to pull out of the if statement early on in phiopt1. */
/* { dg-final { scan-tree-dump "changed to factor operation out from " "phiopt1" } } */
/* { dg-final { scan-tree-dump-not "if " "phiopt1" } } */
/* { dg-final { scan-tree-dump-times "ABS_EXPR " 2 "phiopt1" } } */
/* { dg-final { scan-tree-dump-not "if " "optimized" } } */
/* { dg-final { scan-tree-dump-times "ABS_EXPR " 2 "optimized" } } */
