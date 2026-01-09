/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** sqrt_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	fsqrt	z0\.h, p0/z, \1\.h
**	ret
*/
TEST_UNIFORM_Z (sqrt_f16_z_tied1, svfloat16_t,
		z0 = svsqrt_f16_z (p0, z0),
		z0 = svsqrt_z (p0, z0))

/*
** sqrt_f16_z_untied:
**	fsqrt	z0\.h, p0/z, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sqrt_f16_z_untied, svfloat16_t,
		z0 = svsqrt_f16_z (p0, z1),
		z0 = svsqrt_z (p0, z1))
