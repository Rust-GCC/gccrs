/* { dg-add-options vect_early_break } */
/* { dg-do compile } */
/* { dg-require-effective-target vect_early_break } */
/* { dg-require-effective-target vect_int } */

#define N 1000
int a[N] = {0};
int b[N] = {0};
int c[N] = {0};

int foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i] && a[i] > c[i])
	return 1;
    }
  return 0;
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "early break does not require epilog" "vect" { xfail *-*-* } } } */
