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
**	vqadd.u32	q[0-9]+, q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
uint32x4_t
foo (uint32x4_t a, uint32_t b)
{
  return vqaddq_n_u32 (a, b);
}


/*
**foo1:
**	...
**	vqadd.u32	q[0-9]+, q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
uint32x4_t
foo1 (uint32x4_t a, uint32_t b)
{
  return vqaddq (a, b);
}

/*
**foo2:
**	...
**	vqadd.u32	q[0-9]+, q[0-9]+, (?:ip|fp|r[0-9]+)(?:	@.*|)
**	...
*/
uint32x4_t
foo2 (uint32x4_t a)
{
  return vqaddq (a, 1);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */