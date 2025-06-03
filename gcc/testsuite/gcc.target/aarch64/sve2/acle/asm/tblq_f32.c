/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** tblq_f32_tied1:
**	tblq	z0\.s, {z0\.s}, z4\.s
**	ret
*/
TEST_DUAL_Z (tblq_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtblq_f32 (z0, z4),
	     z0 = svtblq (z0, z4))

/*
** tblq_f32_tied2:
**	tblq	z0\.s, {z4\.s}, z0\.s
**	ret
*/
TEST_DUAL_Z_REV (tblq_f32_tied2, svfloat32_t, svuint32_t,
		 z0_res = svtblq_f32 (z4, z0),
		 z0_res = svtblq (z4, z0))

/*
** tblq_f32_untied:
**	tblq	z0\.s, {z1\.s}, z4\.s
**	ret
*/
TEST_DUAL_Z (tblq_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtblq_f32 (z1, z4),
	     z0 = svtblq (z1, z4))
