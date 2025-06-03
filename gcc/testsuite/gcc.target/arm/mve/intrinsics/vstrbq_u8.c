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
**	vstrb.8	q[0-9]+, \[(?:ip|fp|r[0-9]+)\](?:	@.*|)
**	...
*/
void
foo (uint8_t *base, uint8x16_t value)
{
  vstrbq_u8 (base, value);
}


/*
**foo1:
**	...
**	vstrb.8	q[0-9]+, \[(?:ip|fp|r[0-9]+)\](?:	@.*|)
**	...
*/
void
foo1 (uint8_t *base, uint8x16_t value)
{
  vstrbq (base, value);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */
