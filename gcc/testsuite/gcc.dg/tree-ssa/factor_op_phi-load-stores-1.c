/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt-details" } */
int f(int *a, int *c, int *d, int e)
{
  int t;
  if (e)
  {
    *a = 1;
    t = *c;
  }
  else
  {
    *a = 0;
    t = *d;
  }
  return t;
}

/* { dg-final { scan-tree-dump-times "changed to factor out load from" 1 "phiopt2" } } */
/* { dg-final { scan-tree-dump-times "factoring out stores" 1 "phiopt2" } } */

