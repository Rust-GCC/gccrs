/* { dg-add-options vect_early_break } */
/* { dg-do compile } */
/* { dg-require-effective-target vect_early_break } */
/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-march=armv8-a+sve" { target { aarch64*-*-* } } } */

#define N 1000
int a[N] = {0};
int b[N] = {0};

int foo (void)
{
  for (int i = 0; i < (N / 2); i += 2)
    {
      if (a[i] > b[i])
	return 1;

      if (a[i + 1] > b[i + 1])
	return 1;
    }
  return 0;
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" { target { aarch64*-*-* } } } } */
/* { dg-final { scan-tree-dump-not "early break does not require epilog" "vect" { target { aarch64*-*-* } } } } */
