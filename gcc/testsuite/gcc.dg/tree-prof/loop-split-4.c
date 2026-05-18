/* PR tree-optimization/XXXXX */
/* { dg-options "-O2 -fdump-tree-lsplit-details" } */

volatile int sink;

__attribute__((noinline)) int
helper (int a, int b)
{
  return a + b;
}

int
main (void)
{
  int n = 100, t = 3, total = 0;
  /* With t=3, n=100 the guard "t < i" is false for i=0..3 (4 iterations,
     empty else) and true for i=4..99 (96 iterations, calls helper).
     split_at_bb_p swaps "t < i" to "i > t" (GT_EXPR), giving
     initial_true = false.  Loop1 (cold, 4 iterations) handles the false
     case, loop2 (hot, 96 iterations) handles the true case.
     Without the fix loop1 was scaled by true_edge->probability (96%),
     inverting the counts.  */
  for (int i = 0; i < n; i++)
    if (t < i)
      total += helper (i, t);
  sink = total;
  return 0;
}
/* { dg-final-use-not-autofdo { scan-tree-dump-times "Loop split" 1 "lsplit" } } */
/* { dg-final-use-not-autofdo { scan-tree-dump-times "Invalid sum" 0 "lsplit" } } */
/* { dg-final-use-not-autofdo { scan-tree-dump "Split loop: initial_true false" "lsplit" } } */
/* With the fix loop1 (cold, 4 iterations) count ~4, loop2 (hot, 96
   iterations) count ~96.  Without the fix the counts are inverted.
   Check loop1 is a single-digit and loop2 is 90+.  */
/* { dg-final-use-not-autofdo { scan-tree-dump "loop1 count \[0-9\], loop2 count 9\[0-9\]" "lsplit" } } */
/* The skip edge from loop1 to loop2 should use loop1_prob.invert (),
   where loop1_prob corresponds to the false edge when initial_true is false.  */
/* { dg-final-use-not-autofdo { scan-tree-dump "goto <bb \[0-9\]+>; .99\\." "lsplit" } } */
