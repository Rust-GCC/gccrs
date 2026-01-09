/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** recpx_f32_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	frecpx	z0\.s, p0/z, \1\.s
**	ret
*/
TEST_UNIFORM_Z (recpx_f32_z_tied1, svfloat32_t,
		z0 = svrecpx_f32_z (p0, z0),
		z0 = svrecpx_z (p0, z0))

/*
** recpx_f32_z_untied:
**	frecpx	z0\.s, p0/z, z1\.s
**	ret
*/
TEST_UNIFORM_Z (recpx_f32_z_untied, svfloat32_t,
		z0 = svrecpx_f32_z (p0, z1),
		z0 = svrecpx_z (p0, z1))
