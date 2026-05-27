/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 1000
int a[N] = {0};
int b[N] = {0};

/*
** foo:
**	...
**	cmgt	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldr	w[0-9]+, \[x[0-9]+, w[0-9]+, sxtw 2\]
**	ldr	w[0-9]+, \[x[0-9]+, w[0-9]+, sxtw 2\]
**	cmp	w[0-9]+, w[0-9]+
**	...
**	ret
*/
__attribute__ ((noipa, noinline))
int foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return i;
    }
  return 0;
}
