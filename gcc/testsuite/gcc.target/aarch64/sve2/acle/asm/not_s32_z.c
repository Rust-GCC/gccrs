/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** not_s32_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	not	z0\.s, p0/z, \1\.s
**	ret
*/
TEST_UNIFORM_Z (not_s32_z_tied1, svint32_t,
		z0 = svnot_s32_z (p0, z0),
		z0 = svnot_z (p0, z0))

/*
** not_s32_z_untied:
**	not	z0\.s, p0/z, z1\.s
**	ret
*/
TEST_UNIFORM_Z (not_s32_z_untied, svint32_t,
		z0 = svnot_s32_z (p0, z1),
		z0 = svnot_z (p0, z1))
