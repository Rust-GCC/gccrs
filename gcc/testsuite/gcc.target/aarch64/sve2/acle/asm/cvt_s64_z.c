/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvt_s64_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	fcvtzs	z0\.d, p0/z, \1\.h
**	ret
*/
TEST_DUAL_Z_REV (cvt_s64_f16_z_tied1, svint64_t, svfloat16_t,
		 z0_res = svcvt_s64_f16_z (p0, z0),
		 z0_res = svcvt_s64_z (p0, z0))

/*
** cvt_s64_f16_z_untied:
**	fcvtzs	z0\.d, p0/z, z4\.h
**	ret
*/
TEST_DUAL_Z (cvt_s64_f16_z_untied, svint64_t, svfloat16_t,
	     z0 = svcvt_s64_f16_z (p0, z4),
	     z0 = svcvt_s64_z (p0, z4))

/*
** cvt_s64_f32_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	fcvtzs	z0\.d, p0/z, \1\.s
**	ret
*/
TEST_DUAL_Z_REV (cvt_s64_f32_z_tied1, svint64_t, svfloat32_t,
		 z0_res = svcvt_s64_f32_z (p0, z0),
		 z0_res = svcvt_s64_z (p0, z0))

/*
** cvt_s64_f32_z_untied:
**	fcvtzs	z0\.d, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvt_s64_f32_z_untied, svint64_t, svfloat32_t,
	     z0 = svcvt_s64_f32_z (p0, z4),
	     z0 = svcvt_s64_z (p0, z4))

/*
** cvt_s64_f64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	fcvtzs	z0\.d, p0/z, \1
**	ret
*/
TEST_DUAL_Z_REV (cvt_s64_f64_z_tied1, svint64_t, svfloat64_t,
		 z0_res = svcvt_s64_f64_z (p0, z0),
		 z0_res = svcvt_s64_z (p0, z0))

/*
** cvt_s64_f64_z_untied:
**	fcvtzs	z0\.d, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvt_s64_f64_z_untied, svint64_t, svfloat64_t,
	     z0 = svcvt_s64_f64_z (p0, z4),
	     z0 = svcvt_s64_z (p0, z4))
