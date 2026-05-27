/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 1000
short a[N] = {0};
short b[N] = {0};

/*
** foo:
**	...
**	ld1h	z[0-9]+\.h, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 1\]
**	ld1h	z[0-9]+\.h, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 1\]
**	cmpgt	p[0-9]+\.h, p[0-9]+/z, z[0-9]+\.h, z[0-9]+\.h
**	...
**	ldrsh	w[0-9]+, \[x[0-9]+, x[0-9]+\]
**	ldrsh	w[0-9]+, \[x[0-9]+, x[0-9]+\]
**	...
**	cmp	w[0-9]+, w[0-9]+
**	...
**	ret
**	...
*/
__attribute__ ((noipa, noinline))
short foo (void)
{
  for (unsigned short i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return a[i];
    }
  return 0;
}
