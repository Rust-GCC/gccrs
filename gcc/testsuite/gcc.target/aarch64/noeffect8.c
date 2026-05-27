/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** add:
**	...
**	cmeq	v[0-9]+\.4s, v[0-9]+\.4s, #0
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldr	q[0-9]+, \[x[0-9]+[^\n]*\]
**	...
**	str	q[0-9]+, \[x[0-9]+[^\n]*\]
**	...
**	ldr	w[0-9]+, \[x[0-9]+, x[0-9]+\]
**	ldr	w[0-9]+, \[x[0-9]+, x[0-9]+\]
**	add	w[0-9]+, w[0-9]+, w[0-9]+
**	str	w[0-9]+, \[x[0-9]+, x[0-9]+\]
**	...
**	ret
**	...
*/
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
