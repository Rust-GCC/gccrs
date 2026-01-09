/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cls_s8_z:
**	cls	z0\.b, p0/z, z4\.b
**	ret
*/
TEST_DUAL_Z (cls_s8_z, svuint8_t, svint8_t,
	     z0 = svcls_s8_z (p0, z4),
	     z0 = svcls_z (p0, z4))

