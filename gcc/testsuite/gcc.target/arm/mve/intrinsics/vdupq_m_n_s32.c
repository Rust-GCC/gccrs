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
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vdupt.32	q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
int32x4_t
foo (int32x4_t inactive, int32_t a, mve_pred16_t p)
{
  return vdupq_m_n_s32 (inactive, a, p);
}


/*
**foo1:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vdupt.32	q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
int32x4_t
foo1 (int32x4_t inactive, int32_t a, mve_pred16_t p)
{
  return vdupq_m (inactive, a, p);
}

/*
**foo2:
**	...
**	vmsr	p0, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
**	vpst(?:	@.*|)
**	...
**	vdupt.32	q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
int32x4_t
foo2 (int32x4_t inactive, mve_pred16_t p)
{
  return vdupq_m (inactive, 1, p);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */
