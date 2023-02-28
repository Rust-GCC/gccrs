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
**	vcmla.f16	q[0-9]+, q[0-9]+, q[0-9]+, #270(?:	@.*|)
**	...
*/
float16x8_t
foo (float16x8_t a, float16x8_t b, float16x8_t c)
{
  return vcmlaq_rot270_f16 (a, b, c);
}


/*
**foo1:
**	...
**	vcmla.f16	q[0-9]+, q[0-9]+, q[0-9]+, #270(?:	@.*|)
**	...
*/
float16x8_t
foo1 (float16x8_t a, float16x8_t b, float16x8_t c)
{
  return vcmlaq_rot270 (a, b, c);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */