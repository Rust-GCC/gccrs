/* { dg-require-effective-target arm_v8_1m_mve_ok } */
/* { dg-add-options arm_v8_1m_mve } */
/* { dg-additional-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_mve.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
**foo:
**	...
**	vclz.i16	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
int16x8_t
foo (int16x8_t a)
{
  return vclzq_s16 (a);
}


/*
**foo1:
**	...
**	vclz.i16	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
int16x8_t
foo1 (int16x8_t a)
{
  return vclzq (a);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */