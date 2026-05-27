/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 8
short a[N] = {0};
short b[N] = {0};

/*
** foo:
**	...
**	ldp	q[0-9]+, q[0-9]+, \[x[0-9]+\]
**	cmplt	p[0-9]+\.h, p[0-9]+/z, z[0-9]+\.h, z[0-9]+\.h
**	ptest	p[0-9]+, p[0-9]+\.b
**	cset	w0, any
**	ret
*/
__attribute__ ((noipa, noinline))
short foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}
