/* { dg-do assemble { target aarch64_asm_sve2p1_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p1_ok } } } */
/* { dg-skip-if "" { *-*-* } { "-DSTREAMING_COMPATIBLE" } { "" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sve2p1"

/*
** uzpq1_f16_tied1:
**	uzpq1	z0\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (uzpq1_f16_tied1, svfloat16_t,
		z0 = svuzpq1_f16 (z0, z1),
		z0 = svuzpq1 (z0, z1))

/*
** uzpq1_f16_tied2:
**	uzpq1	z0\.h, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (uzpq1_f16_tied2, svfloat16_t,
		z0 = svuzpq1_f16 (z1, z0),
		z0 = svuzpq1 (z1, z0))

/*
** uzpq1_f16_untied:
**	uzpq1	z0\.h, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (uzpq1_f16_untied, svfloat16_t,
		z0 = svuzpq1_f16 (z1, z2),
		z0 = svuzpq1 (z1, z2))
