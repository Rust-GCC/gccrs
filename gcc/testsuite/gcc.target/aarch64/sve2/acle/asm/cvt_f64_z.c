/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvt_f64_f16_z_tied1:
**	fcvt	z0\.d, p0/z, z0\.h
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_f16_z_tied1, svfloat64_t, svfloat16_t,
		 z0_res = svcvt_f64_f16_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_f16_z_untied:
**	fcvt	z0\.d, p0/z, z4\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_z_untied, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))

/*
** cvt_f64_f32_z_tied1:
**	fcvt	z0\.d, p0/z, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_f32_z_tied1, svfloat64_t, svfloat32_t,
		 z0_res = svcvt_f64_f32_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_f32_z_untied:
**	fcvt	z0\.d, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_z_untied, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))

/*
** cvt_f64_s32_z_tied1:
**	scvtf	z0\.d, p0/z, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_s32_z_tied1, svfloat64_t, svint32_t,
		 z0_res = svcvt_f64_s32_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_s32_z_untied:
**	scvtf	z0\.d, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_z_untied, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))

/*
** cvt_f64_s64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	scvtf	z0\.d, p0/z, \1
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_s64_z_tied1, svfloat64_t, svint64_t,
		 z0_res = svcvt_f64_s64_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_s64_z_untied:
**	scvtf	z0\.d, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_z_untied, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))

/*
** cvt_f64_u32_z_tied1:
**	ucvtf	z0\.d, p0/z, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_u32_z_tied1, svfloat64_t, svuint32_t,
		 z0_res = svcvt_f64_u32_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_u32_z_untied:
**	ucvtf	z0\.d, p0/z, z4\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_z_untied, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))

/*
** cvt_f64_u64_z_tied1:
**	mov	(z[0-9]+\.d), z0\.d
**	ucvtf	z0\.d, p0/z, \1
**	ret
*/
TEST_DUAL_Z_REV (cvt_f64_u64_z_tied1, svfloat64_t, svuint64_t,
		 z0_res = svcvt_f64_u64_z (p0, z0),
		 z0_res = svcvt_f64_z (p0, z0))

/*
** cvt_f64_u64_z_untied:
**	ucvtf	z0\.d, p0/z, z4\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_z_untied, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_z (p0, z4),
	     z0 = svcvt_f64_z (p0, z4))
