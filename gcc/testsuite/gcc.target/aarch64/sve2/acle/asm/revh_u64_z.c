/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** revh_u64_z_tied1:
**	revh	z0\.d, p0/z, z0\.d
**	ret
*/
TEST_UNIFORM_Z (revh_u64_z_tied1, svuint64_t,
		z0 = svrevh_u64_z (p0, z0),
		z0 = svrevh_z (p0, z0))

/*
** revh_u64_z_untied:
**	revh	z0\.d, p0/z, z1\.d
**	ret
*/
TEST_UNIFORM_Z (revh_u64_z_untied, svuint64_t,
		z0 = svrevh_u64_z (p0, z1),
		z0 = svrevh_z (p0, z1))


