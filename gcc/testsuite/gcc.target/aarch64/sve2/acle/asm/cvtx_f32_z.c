/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvtx_f32_f64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	fcvtx	z0\.s, p0/z, \1
**	ret
*/
TEST_DUAL_Z_REV (cvtx_f32_f64_z_tied1, svfloat32_t, svfloat64_t,
		 z0_res = svcvtx_f32_f64_z (p0, z0),
		 z0_res = svcvtx_f32_z (p0, z0))

/*
** cvtx_f32_f64_z_untied:
**	fcvtx	z0\.s, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvtx_f32_f64_z_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvtx_f32_f64_z (p0, z4),
	     z0 = svcvtx_f32_z (p0, z4))
