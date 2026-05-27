/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve --param=aarch64-autovec-preference=sve-only -msve-vector-bits=scalable" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 4
int a[N] = {0, 0, 0, 1};
int b[N] = {0, 0, 0, 1};

/*
** foo:
**	...
**	ldp	q[0-9]+, q[0-9]+, \[x[0-9]+\]
**	cmplt	p[0-9]+\.s, p[0-9]+/z, z[0-9]+\.s, z[0-9]+\.s
**	ptest	p[0-9]+, p[0-9]+\.b
**	cset	w0, any
**	ret
*/
__attribute__ ((noipa, noinline))
int foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}
