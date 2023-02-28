/* { dg-do compile } */
/* { dg-require-effective-target arm_v8_1m_mve_ok } */
/* { dg-options "-O2" } */
/* { dg-add-options arm_v8_1m_mve } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** test:
**...
**	vstrht.32	q0, \[r0\]
**...
**	vstrht.32	q0, \[r0\]
**...
*/

#define TYPE int32x4_t
#define INTRINSIC vstrhq_s32
#define INTRINSIC_P vstrhq_p_s32

#include "pr108177.x"
