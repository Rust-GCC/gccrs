/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvtxnt_f32_f64_z_tied1:
**	fcvtxnt	z0\.s, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvtxnt_f32_f64_z_tied1, svfloat32_t, svfloat64_t,
	     z0 = svcvtxnt_f32_f64_z (z0, p0, z4),
	     z0 = svcvtxnt_f32_z (z0, p0, z4))

/*
** cvtxnt_f32_f64_z_untied:
** (
**	mov	z0\.d, z1\.d
**	fcvtxnt	z0\.s, p0/z, z4\.d
** |
**	fcvtxnt	z1\.s, p0/z, z4\.d
**	mov	z0\.d, z1\.d
** )
**	ret
*/
TEST_DUAL_Z (cvtxnt_f32_f64_z_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvtxnt_f32_f64_z (z1, p0, z4),
	     z0 = svcvtxnt_f32_z (z1, p0, z4))
