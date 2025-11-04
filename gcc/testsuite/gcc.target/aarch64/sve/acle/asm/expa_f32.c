/* { dg-do assemble { target aarch64_asm_ssve-fexpa_ok } } */
/* { dg-do compile { target { ! aarch64_asm_ssve-fexpa_ok } } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#ifdef STREAMING_COMPATIBLE
#pragma GCC target "+ssve-fexpa"
#endif

/*
** expa_f32_tied1:
**	fexpa	z0\.s, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (expa_f32_tied1, svfloat32_t, svuint32_t,
		 z0_res = svexpa_f32 (z0),
		 z0_res = svexpa (z0))

/*
** expa_f32_untied:
**	fexpa	z0\.s, z4\.s
**	ret
*/
TEST_DUAL_Z (expa_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svexpa_f32 (z4),
	     z0 = svexpa (z4))
