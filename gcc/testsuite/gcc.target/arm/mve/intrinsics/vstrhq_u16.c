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
**	vstrh.16	q[0-9]+, \[(?:ip|fp|r[0-9]+)\](?:	@.*|)
**	...
*/
void
foo (uint16_t *base, uint16x8_t value)
{
  vstrhq_u16 (base, value);
}


/*
**foo1:
**	...
**	vstrh.16	q[0-9]+, \[(?:ip|fp|r[0-9]+)\](?:	@.*|)
**	...
*/
void
foo1 (uint16_t *base, uint16x8_t value)
{
  vstrhq (base, value);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */
