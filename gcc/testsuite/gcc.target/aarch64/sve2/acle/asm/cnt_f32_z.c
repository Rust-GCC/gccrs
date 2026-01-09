/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cnt_f32_z:
**	cnt	z0\.s, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cnt_f32_z, svuint32_t, svfloat32_t,
	     z0 = svcnt_f32_z (p0, z4),
	     z0 = svcnt_z (p0, z4))
