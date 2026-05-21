/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 1000
int a[N] = {0};
int b[N] = {0};
int c[N] = {0};

/*
** foo:
**	...
**	add	x[0-9]+, x[0-9]+, 4000
**	add	x[0-9]+, x[0-9]+, 3648
**	mov	w0, 0
**	b	\.L[0-9]+
**	...
**	ldr	w[0-9]+, \[x[0-9]+[^\n]*\]
**	...
**	cmp	w[0-9]+, w[0-9]+
**	...
**	mov	w0, 1
**	ret
**	mov	w0, 0
**	ret
*/
__attribute__ ((noipa, noinline))
int foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i] && a[i] > c[i])
	return 1;
    }
  return 0;
}
