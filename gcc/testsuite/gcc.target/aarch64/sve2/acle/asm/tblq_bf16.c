/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** tblq_bf16_tied1:
**	tblq	z0\.h, {z0\.h}, z4\.h
**	ret
*/
TEST_DUAL_Z (tblq_bf16_tied1, svbfloat16_t, svuint16_t,
	     z0 = svtblq_bf16 (z0, z4),
	     z0 = svtblq (z0, z4))

/*
** tblq_bf16_tied2:
**	tblq	z0\.h, {z4\.h}, z0\.h
**	ret
*/
TEST_DUAL_Z_REV (tblq_bf16_tied2, svbfloat16_t, svuint16_t,
		 z0_res = svtblq_bf16 (z4, z0),
		 z0_res = svtblq (z4, z0))

/*
** tblq_bf16_untied:
**	tblq	z0\.h, {z1\.h}, z4\.h
**	ret
*/
TEST_DUAL_Z (tblq_bf16_untied, svbfloat16_t, svuint16_t,
	     z0 = svtblq_bf16 (z1, z4),
	     z0 = svtblq (z1, z4))
