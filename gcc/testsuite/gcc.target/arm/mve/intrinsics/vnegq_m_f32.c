/* { dg-require-effective-target arm_v8_1m_mve_fp_ok } */
/* { dg-add-options arm_v8_1m_mve_fp } */
/* { dg-additional-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include "arm_mve.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
**foo:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vnegt.f32	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
float32x4_t
foo (float32x4_t inactive, float32x4_t a, mve_pred16_t p)
{
  return vnegq_m_f32 (inactive, a, p);
}


/*
**foo1:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vnegt.f32	q[0-9]+, q[0-9]+(?:	@.*|)
**	...
*/
float32x4_t
foo1 (float32x4_t inactive, float32x4_t a, mve_pred16_t p)
{
  return vnegq_m (inactive, a, p);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */