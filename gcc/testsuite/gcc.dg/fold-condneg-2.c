/* { dg-do compile } */
/* { dg-options "-O2 -ftrapv -fdump-tree-optimized" } */

#define INT_MIN  (-__INT_MAX__ - 1)

int test(int x)
{
  return x != INT_MIN ? -x : INT_MIN;
}

/* The branch should now be eliminated since we emit
   (signed)(-(unsigned)x) which is well-defined.  */
/* { dg-final { scan-tree-dump-not "goto" "optimized" } } */
