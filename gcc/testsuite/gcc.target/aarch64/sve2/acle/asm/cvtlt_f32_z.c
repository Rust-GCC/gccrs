/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvtlt_f32_f16_z_tied1:
**	fcvtlt	z0\.s, p0/z, z0\.h
**	ret
*/
TEST_DUAL_Z_REV (cvtlt_f32_f16_z_tied1, svfloat32_t, svfloat16_t,
		 z0_res = svcvtlt_f32_f16_z (p0, z0),
		 z0_res = svcvtlt_f32_z (p0, z0))

/*
** cvtlt_f32_f16_z_untied:
**	fcvtlt	z0\.s, p0/z, z4\.h
**	ret
*/
TEST_DUAL_Z (cvtlt_f32_f16_z_untied, svfloat32_t, svfloat16_t,
		 z0 = svcvtlt_f32_f16_z (p0, z4),
		 z0 = svcvtlt_f32_z (p0, z4))
