/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p2"
#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+sme2p2"
#endif

/*
** cvt_s16_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z0\.d
**	fcvtzs	z0\.h, p0/z, \1\.h
**	ret
*/
TEST_DUAL_Z_REV (cvt_s16_f16_z_tied1, svint16_t, svfloat16_t,
		 z0_res = svcvt_s16_f16_z (p0, z0),
		 z0_res = svcvt_s16_z (p0, z0))

/*
** cvt_s16_f16_z_untied:
**	fcvtzs	z0\.h, p0/z, z4\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_z_untied, svint16_t, svfloat16_t,
	     z0 = svcvt_s16_f16_z (p0, z4),
	     z0 = svcvt_s16_z (p0, z4))
