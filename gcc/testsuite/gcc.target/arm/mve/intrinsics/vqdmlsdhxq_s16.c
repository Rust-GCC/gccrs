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
**	vqdmlsdhx.s16	q[0-9]+, q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
int16x8_t
foo (int16x8_t inactive, int16x8_t a, int16x8_t b)
{
  return vqdmlsdhxq_s16 (inactive, a, b);
}


/*
**foo1:
**	...
**	vqdmlsdhx.s16	q[0-9]+, q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
int16x8_t
foo1 (int16x8_t inactive, int16x8_t a, int16x8_t b)
{
  return vqdmlsdhxq (inactive, a, b);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */