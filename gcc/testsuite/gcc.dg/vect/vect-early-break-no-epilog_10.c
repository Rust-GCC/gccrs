/* { dg-add-options vect_early_break } */
/* { dg-do compile } */
/* { dg-require-effective-target vect_early_break } */
/* { dg-require-effective-target vect_int } */

#define N 8
short a[N] = {0};
short b[N] = {0};

short foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-tree-dump "early break does not require epilog" "vect" } } */
