/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-require-effective-target aarch64_asm_f64mm_ok } */
/* { dg-additional-options "-march=armv8.2-a+f64mm" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** trn1q_u16_tied1:
**	trn1	z0\.q, z0\.q, z1\.q
**	ret
*/
TEST_UNIFORM_Z (trn1q_u16_tied1, svuint16_t,
		z0 = svtrn1q_u16 (z0, z1),
		z0 = svtrn1q (z0, z1))

/*
** trn1q_u16_tied2:
**	trn1	z0\.q, z1\.q, z0\.q
**	ret
*/
TEST_UNIFORM_Z (trn1q_u16_tied2, svuint16_t,
		z0 = svtrn1q_u16 (z1, z0),
		z0 = svtrn1q (z1, z0))

/*
** trn1q_u16_untied:
**	trn1	z0\.q, z1\.q, z2\.q
**	ret
*/
TEST_UNIFORM_Z (trn1q_u16_untied, svuint16_t,
		z0 = svtrn1q_u16 (z1, z2),
		z0 = svtrn1q (z1, z2))
