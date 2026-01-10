/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvtnt_f32_f64_z_tied1:
**	fcvtnt	z0\.s, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvtnt_f32_f64_z_tied1, svfloat32_t, svfloat64_t,
	     z0 = svcvtnt_f32_f64_z (z0, p0, z4),
	     z0 = svcvtnt_f32_z (z0, p0, z4))

/*
** cvtnt_f32_f64_z_untied:
** (
**	mov	z0\.d, z1\.d
**	fcvtnt	z0\.s, p0/z, z4\.d
** |
**	fcvtnt	z1\.s, p0/z, z4\.d
**	mov	z0\.d, z1\.d
** )
**	ret
*/
TEST_DUAL_Z (cvtnt_f32_f64_z_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvtnt_f32_f64_z (z1, p0, z4),
	     z0 = svcvtnt_f32_z (z1, p0, z4))
