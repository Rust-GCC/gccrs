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
**	vcmul.f32	q[0-9]+, q[0-9]+, q[0-9]+, #180(?:	@.*|)
**	...
*/
float32x4_t
foo (float32x4_t a, float32x4_t b)
{
  return vcmulq_rot180_f32 (a, b);
}


/*
**foo1:
**	...
**	vcmul.f32	q[0-9]+, q[0-9]+, q[0-9]+, #180(?:	@.*|)
**	...
*/
float32x4_t
foo1 (float32x4_t a, float32x4_t b)
{
  return vcmulq_rot180 (a, b);
}

#ifdef __cplusplus
}
#endif

/* { dg-final { scan-assembler-not "__ARM_undef" } } */