/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvtnt_f16_f32_z_tied1:
**	fcvtnt	z0\.h, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvtnt_f16_f32_z_tied1, svfloat16_t, svfloat32_t,
	     z0 = svcvtnt_f16_f32_z (z0, p0, z4),
	     z0 = svcvtnt_f16_z (z0, p0, z4))

/*
** cvtnt_f16_f32_z_untied:
** (
**	mov	z0\.d, z1\.d
**	fcvtnt	z0\.h, p0/z, z4\.s
** |
**	fcvtnt	z1\.h, p0/z, z4\.s
**	mov	z0\.d, z1\.d
** )
**	ret
*/
TEST_DUAL_Z (cvtnt_f16_f32_z_untied, svfloat16_t, svfloat32_t,
	     z0 = svcvtnt_f16_f32_z (z1, p0, z4),
	     z0 = svcvtnt_f16_z (z1, p0, z4))
