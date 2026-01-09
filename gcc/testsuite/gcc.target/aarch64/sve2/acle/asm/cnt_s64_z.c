/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cnt_s64_z:
**	cnt	z0\.d, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cnt_s64_z, svuint64_t, svint64_t,
	     z0 = svcnt_s64_z (p0, z4),
	     z0 = svcnt_z (p0, z4))
