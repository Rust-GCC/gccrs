/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** extw_u64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	uxtw	z0\.d, p0/z, \1
**	ret
*/
TEST_UNIFORM_Z (extw_u64_z_tied1, svuint64_t,
		z0 = svextw_u64_z (p0, z0),
		z0 = svextw_z (p0, z0))

/*
** extw_u64_z_untied:
**	uxtw	z0\.d, p0/z, z1\.d
**	ret
*/
TEST_UNIFORM_Z (extw_u64_z_untied, svuint64_t,
		z0 = svextw_u64_z (p0, z1),
		z0 = svextw_z (p0, z1))
