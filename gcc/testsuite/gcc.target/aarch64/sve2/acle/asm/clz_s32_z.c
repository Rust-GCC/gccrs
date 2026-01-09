/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** clz_s32_z:
**	clz	z0\.s, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (clz_s32_z, svuint32_t, svint32_t,
	     z0 = svclz_s32_z (p0, z4),
	     z0 = svclz_z (p0, z4))
