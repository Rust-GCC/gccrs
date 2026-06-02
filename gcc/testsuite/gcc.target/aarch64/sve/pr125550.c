/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9.5-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include <arm_sve.h>

svfloat16_t foo (float x0, float x1)
{
  return svdupq_n_f16 (x0, x1, x0, x1, x0, x1, x0, x1);
}

/*
** foo:
**	fcvt	h([01]), s\1
**	fcvt	h([01]), s\2
**	zip1	v0\.4h, v0\.4h, v1\.4h
**	mov	z0\.s, s0
**	ret
*/
