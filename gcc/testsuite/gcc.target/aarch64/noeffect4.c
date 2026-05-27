/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 1000
short a[N] = {0};
short b[N] = {0};

/*
** foo:
**	...
**	cmgt	v[0-9]+\.8h, v[0-9]+\.8h, v[0-9]+\.8h
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldrsh	w[0-9]+, \[[^\n]+\]
**	ldrsh	w[0-9]+, \[[^\n]+\]
**	cmp	w[0-9]+, w[0-9]+
**	...
**	ret
*/
__attribute__ ((noipa, noinline))
int foo (void)
{
  for (unsigned short i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return i;
    }
  return 0;
}
