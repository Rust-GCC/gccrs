/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiopt2-raw" } */

/* Try the same pattern with different integer types.  */

long f_long(long a)
{
  if (a == -__LONG_MAX__ - 1)
    return -__LONG_MAX__ - 1;
  return -a;
}

long long f_ll(long long a)
{
  if (a == -__LONG_LONG_MAX__ - 1)
    return -__LONG_LONG_MAX__ - 1;
  return -a;
}

/* { dg-final { scan-tree-dump-times "negate_expr" 2 "phiopt2" } } */
/* { dg-final { scan-tree-dump-not "gimple_cond <" "phiopt2" } } */
