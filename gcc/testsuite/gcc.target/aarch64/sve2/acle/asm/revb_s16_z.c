/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** revb_s16_z_tied1:
**	revb	z0\.h, p0/z, z0\.h
**	ret
*/
TEST_UNIFORM_Z (revb_s16_z_tied1, svint16_t,
		z0 = svrevb_s16_z (p0, z0),
		z0 = svrevb_z (p0, z0))

/*
** revb_s16_z_untied:
**	revb	z0\.h, p0/z, z1\.h
**	ret
*/
TEST_UNIFORM_Z (revb_s16_z_untied, svint16_t,
		z0 = svrevb_s16_z (p0, z1),
		z0 = svrevb_z (p0, z1))


