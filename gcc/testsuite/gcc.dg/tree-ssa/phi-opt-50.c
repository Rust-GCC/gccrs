/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2-raw" } */

/* a == INT_MIN ? INT_MIN : -a  ->  (int)(-(unsigned)a) */
int f1(int a)
{
  if (a == -__INT_MAX__ - 1)
    return -__INT_MAX__ - 1;
  return -a;
}

/* a != INT_MIN ? -a : INT_MIN  ->  (int)(-(unsigned)a) */
int f2(int a)
{
  if (a != -__INT_MAX__ - 1)
    return -a;
  return -__INT_MAX__ - 1;
}

/* { dg-final { scan-tree-dump-times "negate_expr" 2 "phiopt2" } } */
/* { dg-final { scan-tree-dump-not "gimple_cond <" "phiopt2" } } */
