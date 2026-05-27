/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** add:
**	...
**	cmpeq	p[0-9]+\.s, p[0-9]+/z, z[0-9]+\.s, #1
**	...
**	ld1w	z[0-9]+\.s, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 2\]
**	ld1w	z[0-9]+\.s, p[0-9]+/z, \[x[0-9]+, x[0-9]+, lsl 2\]
**	add	z[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s
**	st1w	z[0-9]+\.s, p[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]
**	...
**	ldr	w[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]
**	ldr	w[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]
**	add	w[0-9]+, w[0-9]+, w[0-9]+
**	str	w[0-9]+, \[x[0-9]+, x[0-9]+, lsl 2\]
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
