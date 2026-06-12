/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8.2-a+sve -fdump-tree-phiopt-details -fdump-tree-vect-details" } */

/* Counterpart to scc-diamond-1.c, showing why sink_common_computations_to_bb
   defers in-loop if-conversion until after vectorisation.  The conditional
   loads a[i] / b[i] are affine in the induction variable and do NOT depend on
   any loaded value, so the loop vectorises (masked/blended contiguous loads).
   The late-sink gate (fold_before_rtl_expansion_p) only runs after the
   vectorisers, so the loop is vectorised first; were the diamond instead
   if-converted into one selected-address load it would become a gather and lose
   the cheap contiguous vectorisation.  */

void
f (int *__restrict r, const int *__restrict a, const int *__restrict b,
   const int *__restrict c, int n)
{
  for (int i = 0; i < n; i++)
    {
      int x;
      if (c[i])
	x = a[i];
      else
	x = b[i];
      r[i] = x;
    }
}

/* The loop vectorises, so by the late sink there is no scalar diamond to
   if-convert ... */
/* { dg-final { scan-tree-dump-not "changed to factor out load from" "phiopt1" } } */
/* { dg-final { scan-tree-dump-not "changed to factor out load from" "phiopt2" } } */
/* { dg-final { scan-tree-dump-not "changed to factor out load from" "phiopt3" } } */
/* ... and it vectorises with cheap contiguous loads instead of a gather.  */
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
