/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-phiprop1-details -fdump-tree-release_ssa" } */

/* PR tree-optimization/116823 */
/* The clobber on the temp s2 should not get in the way of phiprop here. */
/* We should have MAX_EXPR early on then too. */
/* This is derived from fold-const.cc; s2 is similar to poly_int. */

struct s2
{
  int i;
  s2(const int &a) : i (a) {}
};


int h(s2 b);

int g(int l, int r)
{
  return h(l > r ? l : r);
}

/* { dg-final { scan-tree-dump-times "Inserting PHI for result of load" 1 "phiprop1"} } */
/* { dg-final { scan-tree-dump-times "MAX_EXPR" 1 "release_ssa"} } */

