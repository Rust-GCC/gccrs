/* { dg-do compile } */
/* { dg-require-effective-target lp64 } */
/* { dg-options "-O3 -march=armv8-a+sve --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** f1:
**	...
**	whilelo	p[0-9]+\.d, x[0-9]+, x[0-9]+
**	...
**	ld1d	z[0-9]+\.d, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 3\]
**	ld1d	z[0-9]+\.d, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 3\]
**	cmplo	p[0-9]+\.d, p[0-9]+/z, z[0-9]+\.d, z[0-9]+\.d
**	...
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+, lsl 3\]
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+, lsl 3\]
**	...
**	cmp	x[0-9]+, x[0-9]+
**	...
**	ret
**	...
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
**	whilelo	p[0-9]+\.d, x[0-9]+, x[0-9]+
**	...
**	ld1d	z[0-9]+\.d, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 3\]
**	ld1d	z[0-9]+\.d, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 3\]
**	cmplo	p[0-9]+\.d, p[0-9]+/z, z[0-9]+\.d, z[0-9]+\.d
**	...
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+, lsl 3\]
**	ldr	x[0-9]+, \[x[0-9]+, x[0-9]+, lsl 3\]
**	...
**	cmp	x[0-9]+, x[0-9]+
**	...
**	ret
**	...
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
