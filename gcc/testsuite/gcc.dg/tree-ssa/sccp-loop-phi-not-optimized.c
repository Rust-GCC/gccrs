/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-sccp-details" } */

int
nested_unused_phi1 (void)
{
  int acc = 0;

  for (int i = 0; i < 20; i++)
    {
      int x = 0;
      for (int j = 0, y = 123; j != i; j++, y = i + 10)
	x = y;
      acc += x;
    }

  return acc;
}

int
nested_unused_phi2 (void)
{
  int acc = 0;

  for (int i = 0; i != 20; i++)
    {
      int x = 0;
      for (int j = 0, y = 123; j != i + 1; j++, y = i + 10)
	x = y;
      acc += x;
    }

  return acc;
}

int
nested_unused_phi3 (void)
{
  int acc = 0;

  for (int i = 0; i != 20; i++)
    {
      int x = 0;
      for (int j = 0, y = 123; j != i + 2; j++, y = i + 10)
	x = y;
      acc += x;
    }

  return acc;
}

/* Expected:
   nested_unused_phi1 -> 123 + sum_{i=2}^{19} (i + 10) = 492
   nested_unused_phi2 -> 123 + sum_{i=1}^{19} (i + 10) = 503
   nested_unused_phi3 -> sum_{i=0}^{19} (i + 10)       = 390

   These functions could be optimized by sccp, but currently are not because
   they are not normal affine recurrence.
   compute_final_value_from_loop_phi_latch tries to optimize these cases but
   fails to do so for nested_unsed_phi1/2 as the niter in the inner loop might
   be zero.
   It should technically be able to optimized nested_unused_phi3 as we know
   that niter > 0 for the inner loop but tree_expr_nonzero_p (niter) returns
   false and we don't.
*/

/* { dg-final { scan-tree-dump-times "not replacing:" 6 "sccp" } } */
