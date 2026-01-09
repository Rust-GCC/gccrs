/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** rintm_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	frintm	z0\.h, p0/z, \1\.h
**	ret
*/
TEST_UNIFORM_Z (rintm_f16_z_tied1, svfloat16_t,
		z0 = svrintm_f16_z (p0, z0),
		z0 = svrintm_z (p0, z0))

/*
** rintm_f16_z_untied:
**	frintm	z0\.h, p0/z, z1\.h
**	ret
*/
TEST_UNIFORM_Z (rintm_f16_z_untied, svfloat16_t,
		z0 = svrintm_f16_z (p0, z1),
		z0 = svrintm_z (p0, z1))
