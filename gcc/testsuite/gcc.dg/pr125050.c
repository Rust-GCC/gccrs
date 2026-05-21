/* PR tree-optimization/125050 */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2" } */

int f(int a)
{
  if (a == -__INT_MAX__ - 1)
    return -__INT_MAX__ - 1;
  return -a;
}

/* This should be converted to (int)(-(unsigned)a).  */
/* { dg-final { scan-tree-dump-not "if " "phiopt2" } } */
