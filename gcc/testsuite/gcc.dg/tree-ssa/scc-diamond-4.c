/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt2-details" } */

/* PR tree-optimization/125557: outside any loop, a diamond that selects between
   two loads PHI <*p, *q> is if-converted by the sink pass
   (sink_common_computations_to_bb): the two conditional loads are commoned into
   a single load of a run-time-selected pointer and the branch is removed.  Out
   of any loop there is nothing to vectorise, so this fires already at the early
   sink.  */

int
f (int *p, int *q, int c)
{
  int x;
  if (c)
    x = *p;
  else
    x = *q;
  return x;
}

/* The two arm loads collapse to a single selected-pointer load, branchless.  */
/* { dg-final { scan-tree-dump "changed to factor out load from COND_EXPR" "phiopt2" } } */
