/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2+bf16"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvt_bf16_f32_z_tied1:
**	bfcvt	z0\.h, p0/z, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (cvt_bf16_f32_z_tied1, svbfloat16_t, svfloat32_t,
		 z0_res = svcvt_bf16_f32_z (p0, z0),
		 z0_res = svcvt_bf16_z (p0, z0))

/*
** cvt_bf16_f32_z_untied:
**	bfcvt	z0\.h, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvt_bf16_f32_z_untied, svbfloat16_t, svfloat32_t,
	     z0 = svcvt_bf16_f32_z (p0, z4),
	     z0 = svcvt_bf16_z (p0, z4))
