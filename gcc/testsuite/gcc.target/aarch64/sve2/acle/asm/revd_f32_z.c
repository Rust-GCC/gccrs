/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** revd_f32_z_tied1:
**	revd	z0\.q, p0/z, z0\.q
**	ret
*/
TEST_UNIFORM_Z (revd_f32_z_tied1, svfloat32_t,
		z0 = svrevd_f32_z (p0, z0),
		z0 = svrevd_z (p0, z0))

/*
** revd_f32_z_untied:
**	revd	z0\.q, p0/z, z1\.q
**	ret
*/
TEST_UNIFORM_Z (revd_f32_z_untied, svfloat32_t,
		z0 = svrevd_f32_z (p0, z1),
		z0 = svrevd_z (p0, z1))
