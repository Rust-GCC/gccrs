/* { dg-add-options vect_early_break } */
/* { dg-do compile } */
/* { dg-require-effective-target vect_early_break } */
/* { dg-require-effective-target vect_int } */

void
add (int n, int *__restrict a, int *__restrict b, int *__restrict c)
{
  for (int i = 0; i < n; i++)
    {
      c[i] = a[i] + b[i];
      if (i > 1000)
	break;
    }
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-tree-dump-not "early break does not require epilog" "vect" } } */
