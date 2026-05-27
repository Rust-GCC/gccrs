/* { dg-do compile } */
/* { dg-require-effective-target lp64 } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** f1:
**	...
**	cmhi	v[0-9]+\.2d, v[0-9]+\.2d, v[0-9]+\.2d
**	...
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+\]
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+\]
**	cmp	x[0-9]+, x[0-9]+
**	...
**	ret
*/
__attribute__ ((noipa))
int f1 (const unsigned long *restrict a, const unsigned long *b, int n)
{
  for (int i = 0; i < n; ++i)
    {
      if (a[i] < b[i])
	return 0;
      if (a[i] > b[i])
	return 1;
    }
  return 1;
}

/*
** f2:
**	...
**	cmhi	v[0-9]+\.2d, v[0-9]+\.2d, v[0-9]+\.2d
**	...
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+\]
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+\]
**	cmp	x[0-9]+, x[0-9]+
**	...
**	ret
*/
__attribute__ ((noipa))
int f2 (const unsigned long *restrict a, const unsigned long *b, int n)
{
  for (int i = 0; i < n; ++i)
    {
      if (a[i] < b[i])
	return 1;
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}

int main (void)
{
  static unsigned long a[3] __attribute__ ((aligned (16))) = {10, 1, 0};
  static unsigned long b[3] __attribute__ ((aligned (16))) = {9, 2, 0};

  if (f1 (a, b, 3) != 1)
    __builtin_abort ();

  if (f2 (a, b, 3) != 1)
    __builtin_abort ();

  return 0;
}
